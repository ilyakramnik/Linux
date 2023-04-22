#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

#define NUM_PAINTINGS 5
#define MAX_VISITORS 10
#define MAX_TIME_ON_PAINTING 3

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Enter: %s num_visitors\n", argv[0]);
        return 1;
    } else if (atoi(argv[1]) > 50) {
        printf("You should input no more than 50 visitors\n");
        return 2;
    }

    int num_visitors = atoi(argv[1]);
    int status;
    int semid;
    int shmid;
    pid_t pid, pids[num_visitors];
    key_t key;
    int *paintings;
    char pathname[] = "main.c";
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if ((shmid = shmget(key, sizeof(int) * NUM_PAINTINGS,
                        0666 | IPC_CREAT | IPC_EXCL) < 0)) { // создаем общую память для всех потоков
        if (errno != EEXIST) {
            printf("Can\'t create shared memory\n");
            exit(-1);
        } else {
            if ((shmid = shmget(key, sizeof(int) * NUM_PAINTINGS, 0)) < 0) {
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
        }
    }

    if (shmid >= 0) {
        if ((paintings = (int *) shmat(shmid, NULL, 0)) == (int *) (-1)) {
            printf("Can't attach shared memory\n");
            exit(-1);
        }
        for (int i = 0; i < NUM_PAINTINGS; ++i) {
            paintings[i] = 0;
        }
    } else {
        shmid = shmget(key, sizeof(int) * NUM_PAINTINGS, 0);
        if ((paintings = (int *) shmat(shmid, NULL, 0)) == (int *) (-1)) {
            printf("Can't attach shared memory\n");
            exit(-1);
        }
    }

    struct sembuf mybuf;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get semid\n");
        exit(-1);
    }
    if (semid >= 0) {
        semctl(semid, 0, SETVAL, 1); // создаем семафор равным 1
    } else {
        semid = semget(key, 1, 0); // подключаемся к существующему семафору
    }

    for (int i = 0; i < num_visitors; ++i) {
        srand(time(NULL)+i);
        pids[i] = fork();
        pid = pids[i];
        if (pid < 0) {
            printf("Fork error\n");
            return 1;
        } else if (pid == 0) {
            int painting_id;
            int visited_paintings[5] = {0}; // показывает, сколько сейчас человек смотрят какую картину
            mybuf.sem_op = -1; // вычесть из значения семафора 1
            semop(semid, &mybuf, 1); // применяем операцию выше к семафору
            while (1) {
                painting_id = rand() % NUM_PAINTINGS;
                // printf("%d %d", painting_id, paintings[painting_id]);
                if (paintings[painting_id] <= MAX_VISITORS) {
                    int visit_time =
                            rand() % MAX_TIME_ON_PAINTING + 1; // рандомное время просмотра картины (от 1 до 3 секунд)
                    ++paintings[painting_id]; // человек начал смотреть картину
                    printf("Visitor %d started viewing painting %d.\n", getpid(), painting_id + 1);
                    sleep(visit_time); // человек смотрит картину
                    // printf("Visitor %d viewed painting %d for %d seconds.\n", getpid(), painting_id + 1, visit_time);
                    --paintings[painting_id]; // человек посмотрел картину и отошел от нее
                    visited_paintings[painting_id] = 1; // человек посмотрел n-ую картину

                    mybuf.sem_op = 1; // прибавить к значению семафора 1
                    semop(semid, &mybuf, 1); // применяем операцию выше к семафору
                } else {
                    printf("Now it is already 10 people watching painting %d, so Visitor %d is waiting\n",
                           painting_id + 1, getpid());
                }

                if (visited_paintings[0] > 0 && visited_paintings[1] > 0 && visited_paintings[2] > 0
                    && visited_paintings[3] > 0 &&
                    visited_paintings[4] > 0) { // если человек посмотрел все картины, то он уходит из галереи
                    exit(0);
                }
            }
        }
    }

    for (int i = 0; i < num_visitors; ++i) {
        waitpid(pids[i], &status, 0); // процесс-родитель ждет завершения процесса-ребенка
    }
    if (pid > 0) {
        printf("Gallery is closed.\n");
    }

    if (shmdt(paintings) < 0) { // исключаем разделяемую память из адресного пространства
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    semctl(semid, 0, IPC_RMID, 0); // удаление семафора
    return 0;
}
