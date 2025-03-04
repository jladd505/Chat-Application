# Chat Application

## Overview
The **Chat Application** is a simple command-line based messaging system implemented in **C** for a class project. The application allows two users to chat with each other in real-time by connecting to a **UNIX server**. This project demonstrates basic client-server communication using **SSH** for remote access, as well as file handling for storing chat logs.

## Requirements
- **UNIX-based OS** (Linux, macOS, or similar)
- **SSH** access to a UNIX server
- **C compiler** (GCC)
- **Make** (for building the application)

## How to Run the Application

### 1. **Setting Up the Server**
   - One user needs to run the **chat_server.c** file.
   - The server should be executed on a UNIX server. Use SSH to connect to the server, then compile and run the server with the following commands:

     ```bash
     gcc -o chat_server chat_server.c
     ./chat_server
     ```

### 2. **Setting Up the Clients**
   - Both users must also SSH into the same server and compile the **chat_client.c** file.
   - Each user needs to open a separate terminal and run the following commands:

     ```bash
     gcc -o chat_client chat_client.c
     ./chat_client
     ```

### 3. **Starting the Chat**
   - Once the server and clients are running, the users can begin chatting.
   - All messages sent between the clients will be shown on the screen.

### 4. **Saving Chat Logs**
   - All chat logs are automatically saved to a file named `chat_history` on the server.
   - When the server is terminated (CTRL + C), the chat history is written to the file for future reference.
