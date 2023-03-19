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
    char *buf = (char *) calloc(5000, sizeof(char));
    char *input_file = (char *) calloc(5000, sizeof(char)); // название файла для чтения
    char *output_file = (char *) calloc(5000, sizeof(char)); // название файла для записи
    char temp;
    char fifo_name[] = "file1.fifo";
    char fifo_name2[] = "file2.fifo";
    ssize_t read_bytes;
    size_t write_size, fifo_size;

    pid_t chpid;

    (void) umask(0);
    /* Попытаемся создать fifo с именем file1.fifo в текущей
    директории */
    if (access(fifo_name, F_OK) == -1) {
        if (mknod(fifo_name, S_IFIFO | 0666, 0) < 0) {
            /* Если создать fifo не удалось, печатаем об этом
            сообщение и прекращаем работу */
            printf("Can\'t create fifo at process 1\n");
            exit(-1);
        }
    }
    // создание fifo2
    (void) umask(0);
    if (access(fifo_name2, F_OK) == -1) {
        if (mknod(fifo_name2, S_IFIFO | 0666, 0) < 0) {
            /* Если создать fifo2 не удалось, печатаем об этом
            сообщение и прекращаем работу */
            printf("Can\'t create fifo at process 2\n");
            exit(-1);
        }
    }

    if ((fd_fifo = open(fifo_name, O_RDONLY)) < 0) {
        /* Если открыть fifo не удалось, печатаем об этом
        сообщение и прекращаем работу */
        printf("Can\'t open FIFO for reading at second process\n");
        exit(-1);
    }
    // Пробуем прочитать из fifo данные в массив
    fifo_size = read(fd_fifo, buf, 5000);
    if (fifo_size < 0) {
        /* Если прочитать не смогли, сообщаем об ошибке
        и завершаем работу */
        printf("Can\'t read string at second process\n");
        exit(-1);
    }
    close(fd_fifo);

    // обработка нашей строки
    for (int i = n1; i <= n1 + (n2 - n1) / 2; ++i) {
        temp = buf[i];
        buf[i] = buf[n2 - (i - n1)];
        buf[n2 - (i - n1)] = temp;
    }
    printf("%s\n", buf);

    if ((fd_fifo2 = open(fifo_name2, O_WRONLY)) < 0) {
        /* Если открыть fifo2 не удалось, печатаем об этом
        сообщение и прекращаем работу */
        printf("Can\'t open FIFO for writing at second process\n");
        exit(-1);
    }
    // Пробуем записать в fifo2 наш buf
    fifo_size = write(fd_fifo2, buf, strlen(buf));
    if (fifo_size != strlen(buf)) {
        /* Если записалось меньшее количество байт, то сообщаем
        об ошибке и завершаем работу */
        printf("Can\'t write all string to fifo2 at second process\n");
        exit(-1);
    }
    /* Закрываем входной поток данных и на этом родитель
    прекращает работу */
    close(fd_fifo2);
}