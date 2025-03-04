#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8081
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

typedef struct {
    int socket;
    struct sockaddr_in address;
    pthread_t tid;
    char name[BUFFER_SIZE];
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_client(client_t *client) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] == NULL) {
            clients[i] = client;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] != NULL && clients[i]->socket == socket) {
            pthread_join(clients[i]->tid, NULL);  // Join the thread before removing the client
            clients[i] = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void broadcast_message(char *message, int sender_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] != NULL && clients[i]->socket != sender_socket) {
            if (send(clients[i]->socket, message, strlen(message), 0) < 0) {
                perror("send");
                continue;
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void save_message_to_file(char *message) {
    pthread_mutex_lock(&file_mutex);
    FILE *file = fopen("chat_history", "a");
    if (file == NULL) {
        perror("fopen");
    } else {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
    pthread_mutex_unlock(&file_mutex);
}

void *handle_client(void *arg) {
    char buffer[BUFFER_SIZE];
    client_t *client = (client_t *)arg;
    int nbytes;

    // Receive the first message as the client's name
    if ((nbytes = recv(client->socket, buffer, BUFFER_SIZE, 0)) <= 0) {
        close(client->socket);
        remove_client(client->socket);
        free(client);
        return NULL;
    }
    buffer[nbytes] = '\0';
    strncpy(client->name, buffer, BUFFER_SIZE);
    printf("Client %d joined as %s\n", client->socket, client->name);

    // Notify others about the new client
    char join_message[BUFFER_SIZE];
    snprintf(join_message, BUFFER_SIZE, "%s has joined the chat\n", client->name);
    broadcast_message(join_message, client->socket);
    save_message_to_file(join_message);

    while ((nbytes = recv(client->socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[nbytes] = '\0';
        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "%s: %s", client->name, buffer);
        printf("%s\n", message);
        save_message_to_file(message);
        broadcast_message(message, client->socket);
    }

    // Notify others about the client leaving
    snprintf(join_message, BUFFER_SIZE, "%s has left the chat\n", client->name);
    broadcast_message(join_message, client->socket);
    save_message_to_file(join_message);

    close(client->socket);
    remove_client(client->socket);
    free(client);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // Initialize clients array
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i] = NULL;
    }

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set server address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Chat server started on port %d\n", PORT);

    while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
            perror("accept");
            continue;
        }
        client_t *new_client = (client_t *)malloc(sizeof(client_t));
        new_client->socket = client_socket;
        new_client->address = client_addr;
        add_client(new_client);

        if (pthread_create(&new_client->tid, NULL, handle_client, (void *)new_client) != 0) {
            perror("pthread_create");
            free(new_client);
            continue;
        }
    }

    close(server_socket);
    return 0;
}
