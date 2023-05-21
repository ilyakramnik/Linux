#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
#define BROADCAST_PORT 12345

int create_socket() {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        perror("Error creating socket");
        exit(1);
    }
    return sock;
}

void set_broadcast_permission(int sock) {
    int broadcast_permission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_permission, sizeof(broadcast_permission)) == -1) {
        perror("Error setting socket options");
        exit(1);
    }
}

struct sockaddr_in create_server_address() {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(BROADCAST_PORT);
    return server_addr;
}

struct sockaddr_in create_client_address() {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("192.168.1.255");
    client_addr.sin_port = htons(BROADCAST_PORT);
    return client_addr;
}

void send_message(int sock, struct sockaddr_in client_addr, char* buffer) {
    if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
        perror("Error sending message");
        exit(1);
    }
}

void receive_message(int sock, char* buffer, struct sockaddr_in* client_addr) {
    socklen_t addr_len = sizeof(*client_addr);
    int str_len = recvfrom(sock, buffer, BUF_SIZE - 1, 0, (struct sockaddr *)client_addr, &addr_len);
    if (str_len == -1) {
        perror("Error receiving message");
        exit(1);
    }
    buffer[str_len] = '\0';
}

int main() {
    int server_socket = create_socket();
    set_broadcast_permission(server_socket);

    struct sockaddr_in server_addr = create_server_address();
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(1);
    }

    struct sockaddr_in client_addr = create_client_address();
    char buffer[BUF_SIZE];

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0';

        send_message(server_socket, client_addr, buffer);

        receive_message(server_socket, buffer, &client_addr);
        printf("Received message from %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
    }

    close(server_socket);
}
