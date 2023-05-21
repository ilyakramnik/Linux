#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_MSG_LENGTH 1000

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in server_address;
    char message[MAX_MSG_LENGTH];
    if (argc != 3) {
        printf("You should write: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error creating client socket\n");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("Error connecting to server\n");
        exit(1);
    }

    while (1) {
        memset(message, 0, MAX_MSG_LENGTH);
        if (read(client_socket, message, MAX_MSG_LENGTH) < 0) {
            printf("Error reading message from server\n");
            break;
        }
        if (strcmp(message, "The End\n") == 0) {
            break;
        }
    }

    close(client_socket);
    return 0;
}
