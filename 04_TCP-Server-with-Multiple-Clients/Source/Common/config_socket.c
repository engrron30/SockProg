#include <stdio.h>
#include <arpa/inet.h>

int init_sock(int *sock_fd)
{
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock_fd < 0) {
        perror("Socket creation error");
        return -1;
    }

    return *sock_fd;
}
