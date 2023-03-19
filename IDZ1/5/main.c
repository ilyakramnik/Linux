#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

const int buf_size = 5000;

int main(int argc, char *argv[]) {
    int fd, fd2, n1, n2, fd_fifo, fd_fifo2;
    n1 = atoi(argv[1]); //первое введенное число
    n2 = atoi(argv[2]); // второе введенное число
    char *buf = (char *) calloc(5000, sizeof(char));
    char *input_file = (char *) calloc(5000, sizeof(char)); // название файла для чтения
    char *output_file = (char *) calloc(5000, sizeof(char)); // название файла для записи
    char temp;
    char fifo_name[] ="file1.fifo";
    char fifo_name2[] ="file2.fifo";
    ssize_t read_bytes;
    size_t write_size, fifo_size;

    pid_t chpid, chpid2;

    strcpy(input_file, argv[3]);
    strcpy(output_file, argv[4]);
    remove(output_file);

    (void)umask(0);
    /* Попытаемся создать fifo с именем file1.fifo в текущей
    директории */
    if(mknod(fifo_name, S_IFIFO | 0666, 0) < 0){
        /* Если создать fifo не удалось, печатаем об этом
        сообщение и прекращаем работу */
        printf("Can\'t create fifo at process 1\n");
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

        // записываем данные из buf в fifo
        if((fd_fifo = open(fifo_name, O_WRONLY)) < 0){
            /* Если открыть fifo не удалось, печатаем об этом
            сообщение и прекращаем работу */
            printf("Can\'t open FIFO for writing at first process\n");
            exit(-1);
        }
        // Пробуем записать в fifo наш buf
        fifo_size = write(fd, buf, strlen(buf));
        if(fifo_size != strlen(buf)){
            /* Если записалось меньшее количество байт, то сообщаем
            об ошибке и завершаем работу */
            printf("Can\'t write all string to FIFO at first process\n");
            exit(-1);
        }
        /* Закрываем входной поток данных и на этом родитель
        прекращает работу */
        close(fd_fifo);
        wait(NULL);
    } else if (chpid == 0) { // второй процесс

        if((fd_fifo = open(fifo_name, O_RDONLY)) < 0){
            /* Если открыть fifo не удалось, печатаем об этом
            сообщение и прекращаем работу */
            printf("Can\'t open FIFO for reading at second process\n");
            exit(-1);
        }
        // Пробуем прочитать из fifo данные в массив
        fifo_size = read(fd_fifo, buf, 5000);
        if(fifo_size < 0){
            /* Если прочитать не смогли, сообщаем об ошибке
            и завершаем работу */
            printf("Can\'t read string at second process\n");
            exit(-1);
        }
        close(fd_fifo2);

        // обработка нашей строки
        for (int i = n1; i <= n1 + (n2 - n1) / 2; ++i) {
            temp = buf[i];
            buf[i] = buf[n2 - (i - n1)];
            buf[n2 - (i - n1)] = temp;
        }
        printf("%s\n", buf);

        // создание fifo2
        (void)umask(0);
        if(mknod(fifo_name2, S_IFIFO | 0666, 0) < 0){
            /* Если создать fifo не удалось, печатаем об этом
            сообщение и прекращаем работу */
            printf("Can\'t create fifo at process 2\n");
            exit(-1);
        }

        chpid2 = fork(); // создание третьего потока
        if (chpid2 > 0) {
            // записываем измененную строку в fifo2
            if((fd_fifo2 = open(fifo_name2, O_WRONLY)) < 0){
                /* Если открыть fifo не удалось, печатаем об этом
                сообщение и прекращаем работу */
                printf("Can\'t open FIFO for writing at second process\n");
                exit(-1);
            }
            // Пробуем записать в fifo2 наш buf
            fifo_size = write(fd_fifo2, buf, strlen(buf));
            if(fifo_size != strlen(buf)){
                /* Если записалось меньшее количество байт, то сообщаем
                об ошибке и завершаем работу */
                printf("Can\'t write all string to fifo2 at second process\n");
                exit(-1);
            }
            /* Закрываем входной поток данных и на этом родитель
            прекращает работу */
            close(fd_fifo2);
        } else if (chpid2 == 0) { // третий процесс

            if ((fd2 = open(output_file, O_CREAT | O_WRONLY, 0666)) <
                0) { //имя файла, что делаем с файлом, какие права предоставляем владельцу, группе и пользователю
                printf("Can\'t open file for writing\n");
                exit(-1);
            }

            // Пробуем прочитать из fifo2 данные в массив
            if((fd_fifo2 = open(fifo_name2, O_RDONLY)) < 0){
                /* Если открыть fifo2 не удалось, печатаем об этом
                сообщение и прекращаем работу */
                printf("Can\'t open fifo2 for reading at third process\n");
                exit(-1);
            }

            fifo_size = read(fd_fifo2, buf, 5000);
            if(fifo_size < 0){
                /* Если прочитать не смогли, сообщаем об ошибке
                и завершаем работу */
                printf("Can\'t read string at third process\n");
                exit(-1);
            }
            close(fd_fifo2);

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
}
