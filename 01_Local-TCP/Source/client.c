#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP_ADDR          "127.0.0.1"
#define SERVER_PORT             8080
#define CLIENT_MSG_STR          "Hello from client!"

int main() {

    // 1. Create the client socket
    int sock = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Prepare the server socket struct to connect
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP_ADDR, &serv_addr.sin_addr);

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
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    printf("Server: %s\n", buffer);

    // Close the socket
    close(sock);
    return 0;
}

