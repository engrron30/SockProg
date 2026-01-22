#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "Common/config_socket.h"
#include "dynarray/dynarray.h"

#define SERVER_MSG_STR              "Acknowledged from server!"
#define SERVER_PORT                 8080
#define CLIENT_MSG_STR_LEN          1024
#define MAX_CLIENT_NUM              2

#define SOCKET_CREATION_FAILED      -1

pthread_t client_thread_id[MAX_CLIENT_NUM];
static int active_clients_num = 0;
pthread_mutex_t active_clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void accept_client_comm(int server_fd, struct sockaddr *server_address);
void *handle_client_comm(void *args);

int main() {

    int server_fd = 0;
    if (init_sock(&server_fd) == SOCKET_CREATION_FAILED)
        exit(EXIT_FAILURE);

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

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr*) server_address, (socklen_t*) &server_addrlen);
        if (client_fd < 0) {
            printf("Server failed to accept client's connection\n");
            exit(EXIT_FAILURE);
        } else if (active_clients_num >= MAX_CLIENT_NUM) {
            printf("Maximum number of clients are connected. Try disconnecting first!\n");
            close(client_fd);
            continue;
        }

        struct client_args_s *args = malloc(sizeof(struct client_args_s));
        if (args == NULL) {
            printf("Failed to allocate memory for client arguments!\n");
            exit(EXIT_FAILURE);
        }

        args->client_fd = client_fd;
        args->client_id = ++active_clients_num;
        printf("[SERVER] Client %d is connected. Waiting for messages...\n", active_clients_num);
        if (pthread_create(&client_thread_id[active_clients_num - 1], NULL, handle_client_comm, (void *) args) != 0) {
            printf("Error in creating thread!\n");
            exit(EXIT_FAILURE);
        }

        pthread_detach(client_thread_id[active_clients_num - 1]);
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
            printf("[SERVER] Sending failed to Client %d.\n", client_args->client_id);
            goto exit;
        }
    }

exit:
    close(client_args->client_fd);

    pthread_mutex_lock(&active_clients_mutex);
    active_clients_num--;
    printf("[SERVER] Client %d disconnected. Active clients are %d.\n", client_args->client_id, active_clients_num);
    pthread_mutex_unlock(&active_clients_mutex);

    free(client_args);
    pthread_exit(NULL);
}

#ifdef HANDLE_CLIENT_COMM_AS_INT_FUNC
int handle_client_comm(int client_fd)
{
    char client_msg[CLIENT_MSG_STR_LEN];
    int client_msg_len, send_ret;

    memset(client_msg, 0, CLIENT_MSG_STR_LEN);
    client_msg_len = recv(client_fd, client_msg, sizeof(client_msg) - 1, 0);
    if (client_msg_len <= 0) {
        printf("[SERVER] Client disconnected.\n");
        return CLIENT_DISCONNECTED;
    }

    client_msg[client_msg_len] = '\0';
    printf("CLIENT: %s\n", client_msg);
    if (send(client_fd, SERVER_MSG_STR, strlen(SERVER_MSG_STR), 0) < 0) {
        printf("[SERVER] Sending failed to Client %d.\n", client_args->client_id);
        return CLIENT_DISCONNECTED;
    }

    return CLIENT_CONNECTED;
}
#endif
