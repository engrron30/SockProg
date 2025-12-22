#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_MSG_STR		"Hello from server"
#define PORT 			8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    int addrlen = sizeof(address);
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    read(new_socket, buffer, 1024);
    printf("Client: %s\n", buffer);
    send(new_socket, SERVER_MSG_STR, strlen(SERVER_MSG_STR), 0);
    printf("Message sent\n");

    close(new_socket);
    close(server_fd);
    return 0;
}

