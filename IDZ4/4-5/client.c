#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define NUM_PAINTINGS 5
#define WATCHING_TIME 2

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Enter: %s <port> <server ip>\n", argv[0]);
        return -1;
    }
    int port = atoi(argv[1]);
    char *ip = argv[2];

    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024];

    // Create client socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &(server_address.sin_addr)) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    printf("Visitor joined the gallery.\n");

    srand(time(NULL));

    int visited_paintings[NUM_PAINTINGS] = {0};

    while (1) {
        int all_viewed = 1;
        for (int i = 0; i < NUM_PAINTINGS; ++i) {
            if (visited_paintings[i] == 0) {
                all_viewed = 0;
                break;
            }
        }

        if (all_viewed) {
            break; // All paintings viewed, exit the loop
        }

        int painting_id = rand() % NUM_PAINTINGS;
        sprintf(buffer, "%d", painting_id);
        if (sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        printf("Started viewing painting %d.\n", painting_id + 1);
        visited_paintings[painting_id]++;

        sleep(rand() % WATCHING_TIME + 1); // Simulate viewing time
    }

    printf("Visitor viewed all paintings and left the gallery.\n");

    // Close client socket
    close(client_socket);

    return 0;
}
