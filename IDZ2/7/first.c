#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>

#define NUM_PAINTINGS 5
#define MAX_VISITORS 10
#define MAX_TIME_ON_PAINTING 3

int shm;
const char *shm_name = "shared_memory";
int *shm_ptr;

int shm_semaphores;
const char *shm_semaphores_name = "shared_memory_semaphores";
sem_t *semaphores;

void clear() {
    if (munmap(shm_ptr, NUM_PAINTINGS) == -1) {
        perror("Incorrect munmap");
    }
    sem_destroy(&semaphores[0]);
    sem_destroy(&semaphores[1]);
}

void handle_sigint(int sig) {
    clear();
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Enter: %s num_visitors\n", argv[0]);
        return 1;
    } else if (atoi(argv[1]) > 50) {
        printf("You should input no more than 50 visitors\n");
        return 2;
    }

    signal(SIGINT, handle_sigint); // обработчик прерывания
    int num_visitors = atoi(argv[1]);
    int status;
    int shmid;

    if ((shm = shm_open(shm_name, O_CREAT | O_RDWR, 0666)) == -1) {
        perror("Can't create shared memory\n");
        exit(-1);
    }

    if (ftruncate(shm, NUM_PAINTINGS) == -1) {
        perror("Can't truncate shared memory\n");
        exit(-1);
    }

    if ((shm_ptr = mmap(0, NUM_PAINTINGS, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0)) == MAP_FAILED) {
        perror("Can't mmap shared memory\n");
        exit(-1);
    }

    for (int i = 0; i < NUM_PAINTINGS; ++i) {
        shm_ptr[i] = 0;
    }

    if ((shm_semaphores = shm_open(shm_semaphores_name, O_CREAT | O_RDWR, 0666)) == -1) {
        perror("Can't create shared memory\n");
        exit(-1);
    }

    if (ftruncate(shm_semaphores, 4096) == -1) {
        perror("Can't truncate shared memory\n");
        exit(-1);
    }

    if ((semaphores = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shm_semaphores, 0)) == MAP_FAILED) {
        perror("Can't mmap shared memory\n");
        exit(-1);
    }
    sem_init(&semaphores[0], 1, 0);
    sem_post(&semaphores[0]); // прибавить к значению семафора 1
    sem_init(&semaphores[1], 1, 0);
    sem_post(&semaphores[1]); // прибавить к значению семафора 1

    for (int i = 0; i < num_visitors; ++i) {
        sem_wait(&semaphores[1]); // вычесть из значения семафора 1
        sem_post(&semaphores[1]); // прибавить к значению семафора 1
    }

    clear();
    return 0;
}
