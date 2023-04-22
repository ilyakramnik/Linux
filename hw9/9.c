#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <errno.h>

int main() {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("Error: Cannot create pipe.\n");
        exit(1);
    }

    sem_t *sem;
    const char *sem_name = "my_semaphore";
    if ((sem = sem_open(sem_name, O_CREAT, 0666, 0)) == SEM_FAILED) {
        perror("Error: Cannot create semaphore.\n");
        exit(1);
    }

    int process = fork();
    if (process < 0) {
        perror("Error: Cannot fork process.\n");
        exit(1);
    } else if (process > 0) { // Родительский процесс
        for (int i = 1; i <= 10; ++i) {
            char parent_message[256];
            sprintf(parent_message, "Parent message #%d", i);

            int size = write(fd[1], parent_message, 256);
            if (size < 0) {
                perror("Error: Cannot write to pipe.\n");
                exit(1);
            }

            // Увеличение счетчика семафора
            if (sem_post(sem) == -1) {
                perror("Error: Cannot post semaphore in parent.\n");
                exit(1);
            }

            // Ожидание счетчика семафора
            if (sem_wait(sem) == -1) {
                perror("Error: Cannot wait semaphore in parent.\n");
                exit(1);
            }

            char buffer[256];
            size = read(fd[0], buffer, 256);
            if (size < 0) {
                perror("Error: Cannot read from pipe.\n");
                exit(1);
            }
            printf("Parent received: %s\n", buffer);
        }

        // Закрытие канала и семафора в родительском процессе
        if (close(fd[0]) == -1) {
            perror("Error: Cannot close pipe in parent.\n");
            exit(1);
        }
        if (close(fd[1]) == -1) {
            perror("Error: Cannot close pipe in parent.\n");
            exit(1);
        }
        if (sem_close(sem) == -1) {
            perror("Error: Cannot close semaphore in parent.\n");
            exit(1);
        }
    } else { // Дочерний процесс
        for (int i = 1; i <= 10; ++i) {
            // Ожидание счетчика семафора
            if (sem_wait(sem) == -1) {
                perror("Error: Cannot wait semaphore in child.\n");
                exit(1);
            }

            char buffer[256];
            int size = read(fd[0], buffer, 256);
            if (size < 0) {
                perror("Error: Cannot read from pipe.\n");
                exit(1);
            }
            printf("Child received: %s\n", buffer);

            char child_message[256];
            sprintf(child_message, "Child message #%d", i);
            size = write(fd[1], child_message, 256);
            if (size < 0) {
                perror("Error: Cannot write to pipe.\n");
                exit(1);
            }

            // Увеличение счетчика семафора
            if (sem_post(sem) == -1) {
                perror("Error: Cannot post semaphore in child.\n");
                exit(1);
            }
        }

        // Закрытие канала и семафора в дочернем процессе
        if (close(fd[0]) == -1) {
            perror("Error: Cannot close pipe in child.\n");
            exit(1);
        }
        if (close(fd[1]) == -1) {
            perror("Error: Cannot close pipe in child.\n");
            exit(1);
        }
        if (sem_close(sem) == -1) {
            perror("Error: Cannot close semaphore in child.\n");
            exit(1);
        }
    }

    // Удаление семафора
    if (sem_unlink(sem_name) == -1) {
        perror("Error: Cannot unlink semaphore.\n");
        exit(1);
    }
}