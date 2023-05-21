#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
#define BROADCAST_PORT 12345

int main() {
    struct sockaddr_in server_addr, client_addr;
    int client_socket, str_len;
    char buffer[BUF_SIZE];

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
	    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(BROADCAST_PORT);

    if (bind(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(1);
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("192.168.1.255");
    client_addr.sin_port = htons(BROADCAST_PORT);

    while (1) {
        socklen_t addr_len = sizeof(server_addr);
        str_len = recvfrom(client_socket, buffer, BUF_SIZE - 1, 0, (struct sockaddr *)&server_addr, &addr_len);
        if (str_len == -1) {
            perror("Error receiving message");
            exit(1);
        }

        buffer[str_len] = '\0';
        printf("Received message from server: %s\n", buffer);

        if (sendto(client_socket, "Got it!", 7, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("Error sending message");
            exit(1);
        }
    }

    close(client_socket);
}

