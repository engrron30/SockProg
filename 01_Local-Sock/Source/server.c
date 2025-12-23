#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_MSG_STR		"Hello from server"
#define PORT 			8080

int main() {

    // 1. Create socket file descriptor
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind the socket to the desired port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // 3. Listen for incoming connections
    listen(server_fd, 3);

    // 4. Accept a client connection
    int new_socket;
    int addrlen = sizeof(address);
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    // 5. Read data from client
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    printf("Client: %s\n", buffer);

    // 6. Send a response to client
    send(new_socket, SERVER_MSG_STR, strlen(SERVER_MSG_STR), 0);
    printf("Message sent\n");

    // 7. Close the connection
    close(new_socket);
    close(server_fd);
    return 0;
}

