#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define task1

#ifdef task1
const int buf_size = 25;
const int mes_size = 14;

int main() {
    int fd[2], result;
    ssize_t size;
    char string[] = "Hello, world!";
    char str_buf[buf_size];

    strcpy(str_buf, "@@@@@@@@@@@@@@");
    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    result = fork();
    if (result < 0) {
        printf("Can't fork child\n");
        exit(-1);
    } else if (result > 0) { // parent process
        if (close(fd[0]) < 0) {
            printf("parent: Can't close reading side of pipe\n");
            exit(-1);
        }

        size = write(fd[1], string, mes_size);
        if (size != mes_size) {
            printf("Can't write all strings to pipe\n");
            exit(-1);
        }

        if (close(fd[1]) < 0) {
            printf("parent: Can't close writing side of pipe\n");
            exit(-1);
        }
        printf("Parent exit\n");
    } else { // child process
        if (close(fd[1]) < 0) {
            printf("child: Can't close writing side of pipe\n");
            exit(-1);
        }
        size = read(fd[0], str_buf, mes_size);
        if (size < 0) {
            printf("Can't read string from pipe\n");
            exit(-1);
        }
        printf("Child exit, str_buf: %s\n", str_buf);
        if (close(fd[0]) < 0) {
            printf("child: Can't close reading side of pipe\n");
            exit(-1);
        }
    }
}
#endif