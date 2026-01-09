#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP_ADDR          "127.0.0.1"
#define SERVER_PORT             8080
#define SERVER_MSG_STR_LEN      1024
#define CLIENT_MSG_STR          "Hello from client!"
#define CLIENT_MSG_STR_LEN      1024

#define SOCKET_CREATION_FAILED  -1

int init_sock(int *sock_fd);
void init_servaddr(struct sockaddr_in *serv_addr);

int main() {

    // 1. Create the client socket
    int sock = 0;
    if (init_sock(&sock) == SOCKET_CREATION_FAILED)
    {
        perror("Socket creation failed!");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;
    init_servaddr(&serv_addr);
    if ((connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
    {
	    perror("Client cannot connect to server!\n");
	    return -1;
    }

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

        // Send the client string to server
        if (send(sock, client_msg, strlen(client_msg), 0) == -1)
        {
            printf("Sending failed!\n");
            break;
        }

        // Read the server's response
        read(sock, server_msg, SERVER_MSG_STR_LEN);
        printf("SERVER: %s\n", server_msg);
    }

    // Close the socket
    close(sock);
    return 0;
}

int init_sock(int *sock_fd)
{
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock_fd < 0) {
        perror("Socket creation error");
        return -1;
    }

    return *sock_fd;
}

void init_servaddr(struct sockaddr_in *serv_addr)
{
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP_ADDR, &serv_addr->sin_addr);
}

/*void send_and_read_msg_to_server()
{
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

        // Send the client string to server
        if (send(sock, client_msg, strlen(client_msg), 0) == -1)
        {
            printf("Sending failed!\n");
            break;
        }

        // Read the server's response
        read(sock, server_msg, SERVER_MSG_STR_LEN);
        printf("SERVER: %s\n", server_msg);
    }
}*/
