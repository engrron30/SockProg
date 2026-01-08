#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_MSG_STR          "Acknowledged from server!"
#define SERVER_PORT             8080
#define CLIENT_MSG_STR_LEN      1024

int main() {

    // 1. Create socket file descriptor
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

#ifdef SERVER_REUSE_ADDR
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        exit(EXIT_FAILURE);
    }
#endif

    // 2. Bind the socket to the desired port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen for incoming connections
    int listen_ret = listen(server_fd, 3);
    if (listen_ret < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("[SERVER] TCP server is listening...\n");

    // 4. Accept a client connection
    int addrlen = sizeof(address);
    int client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    printf("[SERVER] Client is connected. Waiting for messages...\n");

    // 5. Read data from client
    char client_msg[CLIENT_MSG_STR_LEN];
    while (1)
    {
        memset(client_msg, 0, CLIENT_MSG_STR_LEN);

        int client_msg_len = recv(client_fd, client_msg, sizeof(client_msg) - 1, 0);
        if (client_msg_len <= 0) {
            printf("[SERVER] Client disconnected.\n");
            break;
        }

        client_msg[client_msg_len] = '\0';
        printf("CLIENT: %s\n", client_msg);
        send(client_fd, SERVER_MSG_STR, strlen(SERVER_MSG_STR), 0);
    }

    // 7. Close the connection
    close(client_fd);
    close(server_fd);
    return 0;
}

