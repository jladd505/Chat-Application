#Compiler and flags
CC = gcc
CFLAGS = -Wall -g -lpthread -Wno-format-truncation

#targets
CLIENT = client
SERVER = server

all: $(CLIENT) $(SERVER)

#client build
$(CLIENT): chat_client.c
        $(CC) $(CFLAGS) -o $(CLIENT) chat_client.c

#server build
$(SERVER): chat_server.c
        $(CC) $(CFLAGS) -o $(SERVER) chat_server.c

clean:
        rm -f $(CLIENT) $(SERVER)
