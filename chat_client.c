#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <pthread.h>

#define PortNumber 8081
#define BuffSize 1000
#define ServerIP "127.0.0.1"

int sock;
char username[100];

void *sendMsg(void* arg) {
        char buffer[BuffSize];
        printf("Enter a message: ");

        while (1) {
                fgets(buffer, BuffSize, stdin);         //reads input from user

                buffer[strcspn(buffer, "\n")] = '\0';   //removes newline character

                if (strcmp(buffer, "exit") == 0) {      //check if user wants to exit
                        printf("Closing...\n");
                        close(sock);
                        exit(0);
                }

                //send user's message to the server
                ssize_t sentBytes = send(sock, buffer, strlen(buffer), 0);

                if (sentBytes == -1) {
                        perror("Error: message did not send");
                        exit(1);
                }
        }

        return NULL;
}

void * receiveMsg(void* arg) {
        char buffer[BuffSize];
        ssize_t received;

        while (1) {
                memset(buffer, 0, BuffSize);            //clear buffer before proceeding                
                
                received = recv(sock, buffer, BuffSize, 0); //receive msg from server
                if (received == -1) {                   //if there's an error receiving the message
                        perror("Error: message was not received");
                        exit(1);
                }
                printf("%s\n", buffer);                 //print the message from the server
        }
        return NULL;
}

int main() {
        pthread_t send_thread, receive_thread;          //thread variables
        struct sockaddr_in serverAddy;                  //holds server address info

        //socket creation
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
                perror("Error: Socket was not made");   //error msg printed if socket fails
                exit(1);
                }

        //server address
        serverAddy.sin_family = AF_INET;
        serverAddy.sin_port = htons(PortNumber);

        int resultAddy = inet_pton(AF_INET, ServerIP, &serverAddy.sin_addr); // conversion to binary, stores it in serverAddy.sin_addr

        if (resultAddy <= 0) {
                perror("Error: Invalid address");
                exit(1);
        }

        //connect to server
        int connectionStatus = connect(sock, (struct sockaddr*) &serverAddy, sizeof(serverAddy));
        if (connectionStatus == -1) {
                perror("Error: Unable to connect to server");
        exit(1);
}

        //get username
        printf("Enter your username: ");
        fgets(username, sizeof(username), stdin);       //read username input
        username[strcspn(username, "\n")] = '\0';       //remove newline character


        //send username to server
        ssize_t sent = send(sock, username, strlen(username), 0);
        if (sent == -1) {
                perror("Error: failed to send username to the server");
                exit(1);
        }

        printf("'%s' has connected to the server.\n", username);

        //create independent threads
        pthread_create(&send_thread, NULL, sendMsg, NULL);
        pthread_create(&receive_thread, NULL, receiveMsg, NULL);

        //wait for threads to all finish
        pthread_join(send_thread, NULL);
        pthread_join(receive_thread, NULL);

        close(sock);                                    //socket to close after threads are completed

        return 0;
}
