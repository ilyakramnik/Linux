#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

const int size = 1000000;

int main() {
    int fd;
    ssize_t read_bytes;
    size_t size1;
    char buf[size];

    if ((fd = open("my_file", O_RDONLY)) <
        0) { //открываем файл на чтение
        printf("Can\'t open file\n");
        exit(-1);
    }

    do {
        read_bytes = read(fd, buf, size); //считываем данные в буфер
        if (read_bytes == -1) {
            printf("Can\'t write this file\n");
            exit(-1);
        }
        buf[read_bytes] = '\0';
        printf("%s\n", buf); //вывод буфера в консоль для проверки
    } while (read_bytes == size);

    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }

    if ((fd = open("my_file_new", O_WRONLY, 0, 666)) <
        0) {  //открываем файл на запись
        printf("Can\'t open file\n");
        exit(-1);
    }
    size1 = write(fd, buf, read_bytes); //записываем в файл данные из буфера (без мусора)
    if (size1 != read_bytes) {
        printf("Can\'t write all string\n");
        exit(-1);
    }
    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }
    return 0;
}