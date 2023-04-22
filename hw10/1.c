#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

pid_t receiver_pid;
int data;
int bits_left = 0;

void send_bit(int bit) {
    if (bit) {
        kill(receiver_pid, SIGUSR2);
    } else {
        kill(receiver_pid, SIGUSR1);
    }
}

void send_data(int data) {
    for (int i = 0; i < 8; ++i) {
        send_bit(data & 1);
        data >>= 1;
    }
}

void sigusr1_handler(int signum) {
    ++bits_left;
    if (bits_left > 0) {
        send_bit(data & 1);
        data >>= 1;
        --bits_left;
    }
}

void sigusr2_handler(int signum) {
    ++bits_left;
    if (bits_left > 0) {
        send_bit(data & 1);
        data >>= 1;
        --bits_left;
    }
}

int main() {
    printf("Transmitter PID: %d\n", getpid());

    printf("Enter receiver PID: ");
    scanf("%d", &receiver_pid);

    printf("Enter data: ");
    scanf("%d", &data);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    send_data(data);

    while (bits_left > 0) {
        sleep(1);
    }

    kill(receiver_pid, SIGINT);

    printf("Data transmission complete.\n");
    return 0;
}
