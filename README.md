Chat Application
Overview
The Chat Application is a simple command-line based messaging system implemented in C for a class project. The application allows two users to chat with each other in real-time by connecting to a UNIX server. This project demonstrates basic client-server communication using SSH for remote access, as well as file handling for storing chat logs.

Requirements
UNIX-based OS (Linux, macOS, or similar)
SSH access to a UNIX server
C compiler (GCC)
Make (for building the application)
How to Run the Application
1. Setting Up the Server
One user needs to run the chat_server.c file.

The server should be executed on a UNIX server. Use SSH to connect to the server, then compile and run the server with the following commands:

bash
Copy
Edit
gcc -o chat_server chat_server.c
./chat_server
2. Setting Up the Clients
Both users must also SSH into the same server and compile the chat_client.c file.

Each user needs to open a separate terminal and run the following commands:

bash
Copy
Edit
gcc -o chat_client chat_client.c
./chat_client
3. Starting the Chat
Once the server and clients are running, the users can begin chatting.
All messages sent between the clients will be shown on the screen.
4. Saving Chat Logs
All chat logs are automatically saved to a file named chat_history on the server.
When the server is terminated (CTRL + C), the chat history is written to the file for future reference.
File Structure
bash
Copy
Edit
ChatApp/
├── chat_server.c        # Server-side code
├── chat_client.c        # Client-side code
├── Makefile             # Build instructions
└── chat_history         # File where chat logs are saved
Makefile
The Makefile is designed to help with compiling and cleaning up the project. It includes:

Build Function: Compiles both chat_server.c and chat_client.c.
Cleanup Rule: Cleans up the generated files, such as the compiled binaries.
To build the project:
bash
Copy
Edit
make
To clean up generated files:
bash
Copy
Edit
make clean
To remove the compiled binaries and other temporary files:
bash
Copy
Edit
make cleanup
Additional Notes
The chat_history file will contain all the messages exchanged during the chat session, saved when the server is terminated.
Ensure that both users connect to the server and are able to run the client and server executables on the UNIX server.
The server will handle multiple clients, and the chat will continue as long as the server is running.
License
This project is part of a class assignment. Feel free to use, modify, or distribute it under the terms of your class's license agreement.
