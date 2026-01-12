#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_MSG_STR              "Acknowledged from server!"
#define SERVER_PORT                 8080
#define CLIENT_MSG_STR_LEN          1024
#define MAX_CLIENT_NUM              10

#define SOCKET_CREATION_FAILED      -1

void accept_client_comm(int server_fd, struct sockaddr *server_address);
void *handle_client_comm(void *args);

int main() {

    // 1. Create socket file descriptor
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == SOCKET_CREATION_FAILED) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

#ifdef SERVER_REUSE_ADDR
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Socket failed to be reused!");
        exit(EXIT_FAILURE);
    }
#endif

    // 2. Bind the socket to the desired port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] TCP server is listening...\n");
    accept_client_comm(server_fd, (struct sockaddr*) &server_address);

    close(server_fd);
    return 0;
}

typedef enum {
    CLIENT_DISCONNECTED,
    CLIENT_CONNECTED
} client_state;

struct client_args_s {
    int client_fd;
    int client_id;
};

void accept_client_comm(int server_fd, struct sockaddr *server_address)
{
    int server_addrlen = sizeof(*server_address);
    int client_num = 0;
    pthread_t client_thread_id[MAX_CLIENT_NUM];

    while (1) {
        struct client_args_s *args = malloc(sizeof(struct client_args_s));
        if (args == NULL) {
            printf("Failed to allocate memory for client arguments!\n");
            exit(EXIT_FAILURE);
        }

        int client_fd = accept(server_fd, (struct sockaddr*) server_address, (socklen_t*) &server_addrlen);
        if (client_fd < 0) {
            printf("Server failed to accept client's connection\n");
            exit(EXIT_FAILURE);
        }

        client_num++;
        args->client_fd = client_fd;
        args->client_id = client_num;
        printf("[SERVER] Client %d is connected. Waiting for messages...\n", client_num);
        if (pthread_create(&client_thread_id[client_num - 1], NULL, handle_client_comm, (void *) args) != 0) {
            printf("Error in creating thread!\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < client_num; i++) {
        if (pthread_join(client_thread_id[i], NULL) != 0) {
            printf("Error in waiting other threads to get finished!\n");
            exit(EXIT_FAILURE);
        }
    }
}

void *handle_client_comm(void *args)
{
    struct client_args_s *client_args = (struct client_args_s *) args;
    char client_msg[CLIENT_MSG_STR_LEN];
    int client_msg_len, send_ret;

    memset(client_msg, 0, CLIENT_MSG_STR_LEN);
    while (1) {
        client_msg_len = recv(client_args->client_fd, client_msg, sizeof(client_msg) - 1, 0);
        if (client_msg_len <= 0) {
            printf("[SERVER] Client %d disconnected.\n", client_args->client_id);
            goto exit;
        }

        client_msg[client_msg_len] = '\0';
        printf("CLIENT %d: %s\n", client_args->client_id, client_msg);
        send_ret = send(client_args->client_fd, SERVER_MSG_STR, strlen(SERVER_MSG_STR), 0);
        if (send_ret < 0) {
            printf("[SERVER] Sending failed to client %d\n.", client_args->client_id);
            goto exit;
        }
    }

exit:
    close(client_args->client_fd);
    free(client_args);
}

#ifdef HANDLE_CLIENT_COMM_AS_INT_FUNC
int handle_client_comm(int client_fd)
{
    char client_msg[CLIENT_MSG_STR_LEN];
    int client_msg_len;

    memset(client_msg, 0, CLIENT_MSG_STR_LEN);
    client_msg_len = recv(client_fd, client_msg, sizeof(client_msg) - 1, 0);
    if (client_msg_len <= 0) {
        printf("[SERVER] Client disconnected.\n");
        return CLIENT_DISCONNECTED;
    }

    client_msg[client_msg_len] = '\0';
    printf("CLIENT: %s\n", client_msg);
    send(client_fd, SERVER_MSG_STR, strlen(SERVER_MSG_STR), 0);

    return CLIENT_CONNECTED;
}
#endif
