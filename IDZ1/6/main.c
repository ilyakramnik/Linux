#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

const int buf_size = 5000;

int main(int argc, char *argv[]) {
    int fd, fd2, n1, n2, fd_pipe[2], fd_pipe2[2];
    n1 = atoi(argv[1]); //первое введенное число
    n2 = atoi(argv[2]); // второе введенное число
    char *buf = (char *) calloc(5000, sizeof(char));
    char *input_file = (char *) calloc(5000, sizeof(char)); // название файла для чтения
    char *output_file = (char *) calloc(5000, sizeof(char)); // название файла для записи
    char temp;
    int *status;
    ssize_t read_bytes;
    size_t write_size, pipe_size;

    pid_t chpid;

    strcpy(input_file, argv[3]);
    strcpy(output_file, argv[4]);
    remove(output_file);

    if (pipe(fd_pipe) < 0) { // создание pipe
        /* Если создать pipe не удалось, печатаем об этом сообщение
        и прекращаем работу */
        printf("Can\'t create pipe\n");
        exit(-1);
    }
    if (pipe(fd_pipe2) < 0) { // создание второго pipe
        /* Если создать pipe2 не удалось, печатаем об этом сообщение
        и прекращаем работу */
        printf("Can\'t create pipe2\n");
        exit(-1);
    }

    chpid = fork(); // создание второго процесса

    if (chpid < 0) {
        printf("Error with fork\n");
    } else if (chpid > 0) { // первый процесс
        if ((fd = open(input_file, O_CREAT | O_RDONLY, 0666)) <
            0) { //имя файла, что делаем с файлом, какие права предоставляем владельцу, группе и пользователю
            printf("Can\'t open file for reading\n");
            exit(-1);
        }
        do {
            read_bytes = read(fd, buf, buf_size);
            if (read_bytes == -1) {
                printf("Can\'t write this file\n");
                exit(-1);
            }
            buf[read_bytes] = '\0';
        } while (read_bytes == buf_size); // считываем данные в буфер
        if (close(fd) < 0) {
            printf("Can\'t close file\n");
        }

        // записываем данные из buf в pipe
        close(fd_pipe[0]);
        pipe_size = write(fd_pipe[1], buf, strlen(buf));
        printf("%s\n", buf);
        if (pipe_size != strlen(buf)) {
            /* Если записалось меньшее количество байт, сообщаем
            об ошибке и завершаем работу */
            printf("parent Can\'t write all string\n");
            exit(-1);
        }
        /* Закрываем входной поток данных, на этом
        родитель прекращает работу */
        close(fd_pipe[1]);
    }
    else if (chpid == 0) { // второй процесс
        // Пробуем прочитать из pipe данные в массив
        close(fd_pipe[1]);
        pipe_size = read(fd_pipe[0], buf, 5000);
        if (pipe_size < 0) {
            /* Если прочитать не смогли, сообщаем об ошибке и
            завершаем работу */
            printf("Can\'t read string\n");
            exit(-1);
        }

        // обработка нашей строки
        for (int i = n1; i <= n1 + (n2 - n1) / 2; ++i) {
            temp = buf[i];
            buf[i] = buf[n2 - (i - n1)];
            buf[n2 - (i - n1)] = temp;
        }

        // записываем измененную строку во второй pipe
        close(fd_pipe2[0]);
        pipe_size = write(fd_pipe2[1], buf, strlen(buf));
        if (pipe_size != strlen(buf)) {
            /* Если записалось меньшее количество байт, сообщаем
            об ошибке и завершаем работу */
            printf("child Can\'t write all string\n");
            exit(-1);
        }
        /* Закрываем входной поток данных, на этом
        родитель прекращает работу */
        close(fd_pipe2[1]);
    }

    if (chpid > 0) { // возвращаемся в первый процесс
        wait(status);
        // Пробуем прочитать из второго pipe данные в массив
        close(fd_pipe2[1]);
        pipe_size = read(fd_pipe2[0], buf, strlen(buf));
        if (pipe_size < 0) {
            /* Если прочитать не смогли, сообщаем об ошибке и
            завершаем работу */
            printf("Can\'t read string\n");
            exit(-1);
        }
        close(fd_pipe2[0]);
        printf("%s\n", buf);

        if ((fd2 = open(output_file, O_CREAT | O_WRONLY, 0666)) <
            0) { //имя файла, что делаем с файлом, какие права предоставляем владельцу, группе и пользователю
            printf("Can\'t open file for writing\n");
            exit(-1);
        }

        // записываем измененную строку в новый файл
        write_size = write(fd2, buf, strlen(buf));

        if (write_size != strlen(buf)) {
            printf("2nd child Can\'t write all string\n");
            exit(-1);
        }

        if (close(fd2) < 0) {
            printf("Can\'t close file\n");
        }
    }
}
