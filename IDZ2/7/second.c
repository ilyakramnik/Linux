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

int main() {
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

    srand(time(NULL));
    int painting_id;
    int visited_paintings[5] = {0};  // показывает, сколько картин посмотрел данный посетитель
    sem_wait(&semaphores[0]); // вычесть из значения семафора 1
    while (1) {
        painting_id = rand() % NUM_PAINTINGS;
        if (shm_ptr[painting_id] <= MAX_VISITORS) {
            int visit_time =
                    rand() % MAX_TIME_ON_PAINTING + 1; // рандомное время просмотра картины (от 1 до 3 секунд)
            ++shm_ptr[painting_id]; // человек начал смотреть картину
            printf("Visitor %d started viewing painting %d.\n", getpid(), painting_id + 1);
            sleep(visit_time); // человек смотрит картину
            // printf("Visitor %d viewed painting %d for %d seconds.\n", getpid(), painting_id + 1, visit_time);
            --shm_ptr[painting_id]; // человек посмотрел картину и отошел от нее
            visited_paintings[painting_id] = 1; // человек посмотрел n-ую картину
        } else {
            printf("Now it is already 10 people watching painting %d, so Visitor %d is waiting\n",
                   painting_id + 1, getpid());
        }

        if (visited_paintings[0] > 0 && visited_paintings[1] > 0 && visited_paintings[2] > 0
            && visited_paintings[3] > 0 &&
            visited_paintings[4] > 0) { // если человек посмотрел все картины, то он уходит из галереи
            printf("Visitor %d viewed all paintings and left the gallery.\n", getpid());
            break;
        }
    }
    printf("Gallery is closed.\n");
    return 0;
}
