#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "Common/config_socket.h"

#define SERVER_IP_ADDR          "127.0.0.1"
#define SERVER_PORT             8080
#define SERVER_MSG_STR_LEN      1024
#define CLIENT_MSG_STR          "Hello from client!"
#define CLIENT_MSG_STR_LEN      1024

#define SOCKET_CREATION_FAILED  -1

void init_servaddr(struct sockaddr_in *serv_addr);
int read_server_response(int sockfd, char *server_msg);

typedef enum {
    SERVER_DISCONNECTED = 0,
    SERVER_CONNECTED
} server_state;

int main() {

    // 1. Create the client socket
    int client_fd = 0;
    if (init_sock(&client_fd) == SOCKET_CREATION_FAILED)
        exit(EXIT_FAILURE);

    struct sockaddr_in serv_addr;
    init_servaddr(&serv_addr);
    if ((connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
	    perror("Client cannot connect to server");
	    return -1;
    }
    printf("[CLIENT] Client is successfully connected to server!\n");

    // 3. Send data to server
    char client_msg[CLIENT_MSG_STR_LEN];
    char server_msg[SERVER_MSG_STR_LEN];
    while (1)
    {
        memset(client_msg, '\0', sizeof(client_msg));
        memset(server_msg, '\0', sizeof(server_msg));

        // Create client message from user's input
        printf("CLIENT: ");
        fgets(client_msg, sizeof(client_msg), stdin);
        client_msg[strcspn(client_msg, "\n")] = 0;

        if (strcmp(client_msg, "exit") == 0)
            break;
        else if (strlen(client_msg) == 0)
            continue;

        // Send the client string to server
        if (send(client_fd, client_msg, strlen(client_msg), 0) == -1) {
            perror("Sending failed!");
            break;
        }

        // Read the server's response
        if (read_server_response(client_fd, server_msg) == -1) {
            break;
        }

        printf("SERVER: %s\n", server_msg);
    }

    // Close the socket
    close(client_fd);
    return 0;
}

/*int init_sock(int *sock_fd)
{
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock_fd < 0) {
        perror("Socket creation error");
        return -1;
    }

    return *sock_fd;
}*/

void init_servaddr(struct sockaddr_in *serv_addr)
{
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP_ADDR, &serv_addr->sin_addr);
}

#define WAIT_SERVER_RESPONSE_SECS       5
#define WAIT_SERVER_RESPONSE_USECS      0

int read_server_response(int sockfd, char *server_msg)
{
    fd_set readfds;
    struct timeval timeout;
    int server_msg_len;

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    timeout.tv_sec = WAIT_SERVER_RESPONSE_SECS;
    timeout.tv_usec = WAIT_SERVER_RESPONSE_USECS;

    int ret = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
    if (ret == -1) {
        perror("Select() failed");
        return -1;
    } else if (ret == 0) {
        printf("[CLIENT] Timeout! No response received.\n");
        return -1;
    }

    server_msg_len = read(sockfd, server_msg, SERVER_MSG_STR_LEN - 1);
    if (server_msg_len == 0) {
        printf("[CLIENT] Cannot connect to server!\n");
        return -1;
    } else if (server_msg_len < 0) {
        printf("[CLIENT] Error in read\n");
        return -1;
    }

    return 0;
}
