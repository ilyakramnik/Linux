#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

pid_t tx_pid;
int bit_index = 0;
int num_received_bits = 0;
int received_data = 0;
int receiving = 0;

void sigusr1_handler(int signum){
    if (receiving){
        received_data |= (1 << (7 - bit_index));
        ++num_received_bits;
        ++bit_index;
        kill(tx_pid, SIGUSR1);
    }
}

void sigusr2_handler(int signum){
    if (receiving){
        ++num_received_bits;
        ++bit_index;
        kill(tx_pid, SIGUSR1);
    }
}

void sigint_handler(int signum){
    printf("Received data: %d\n", received_data);
    exit(0);
}

int main(){
    printf("Enter transmitter PID: ");
    scanf("%d", &tx_pid);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    signal(SIGINT, sigint_handler);

    printf("Receiver PID: %d\n", getpid());
    receiving = 1;
    kill(tx_pid, SIGUSR1);
    while (1){
        pause();
    }
}
