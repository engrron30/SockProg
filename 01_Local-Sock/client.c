#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT			8080
#define CLIENT_MSG_STR		"Hello from client!"

int main() {
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // 1. Create the client socket
    int sock = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Convert IPv4 and IPv6 addresses from text to binary form
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // 2. Connect to server
    int connect_ret = 0;
    connect_ret = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (connect_ret < 0) {
	    printf("Connection failed!\n");
	    return -1;
    }

    // 3. Send data to server
    send(sock, CLIENT_MSG_STR, strlen(CLIENT_MSG_STR), 0);
    
    // 4. Read the server's response
    read(sock, buffer, 1024);
    printf("Server: %s\n", buffer);

    // Close the socket
    close(sock);
    return 0;
}

