#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ws2tcpip.h>

#define BUFFER_SIZE 1024

int client_socket;
int server_socket;

int is_client_fail(){
    if (client_socket == -1) {
        perror("Accept failed");
        close(server_socket);
        return 1;
    }
}

int is_listen_fail(){
    if (listen(server_socket, 1) == -1) {
        perror("Listen failed");
        close(server_socket);
        return 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("You should write: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket == -1) {
        perror("Accept failed");
        close(server_socket);
        return 1;
    }
    is_client_fail();
    is_listen_fail();

    char buffer[BUFFER_SIZE];
    int bytes_received;
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("Received message: %s\n", buffer);
    }
    close(client_socket);
    close(server_socket);
    return 0;
}
