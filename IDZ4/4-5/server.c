#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAX_VISITORS 10
#define NUM_PAINTINGS 5

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Enter: %s <port>\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);

    int server_socket, address_length;
    struct sockaddr_in server_address, client_address;
    int visitor_count[NUM_PAINTINGS] = {0};

    // Create server socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Bind the server socket to a specific address and port
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("The gallery is opened.\n");

    while (1) {
        // Receive painting ID from client
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        socklen_t client_address_length = sizeof(client_address);
        if (recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_address_length) == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        int painting_id = atoi(buffer);
        printf("Visitor is viewing painting %d.\n", painting_id + 1);

        // Check if the painting has less than 10 visitors
        if (visitor_count[painting_id] < MAX_VISITORS) {
            visitor_count[painting_id]++;
            sprintf(buffer, "Visitor is viewing painting %d.", painting_id + 1);
        } else {
            sprintf(buffer, "Now there are already 10 people watching painting %d, so Visitor is waiting.", painting_id + 1);
        }

        // Send status message to the client
        if (sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&client_address, client_address_length) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}
