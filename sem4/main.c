#define task2

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef task1
const int len = 14;

int main() {
    int fd;
    size_t size;
    char string[] = "Hello, world!";

    (void)umask(0); //(0333), (0777)

    if ((fd = open("my_file", O_WRONLY | O_CREAT, 0, 666)) <
        0) { //имя файла, что делаем с файлом, какие права предоставляем владельцу, группе и пользователю
        printf("Can\'t open file\n");
        exit(-1);
    }

    size = write(fd, string, len);

    if (size != len) {
        printf("Can\'t write all string\n");
        exit(-1);
    }

    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }
    return 0;
}

#endif

#ifdef task2
const int len = 14;
const int size = 4096;

int main(){
    int fd;
    char buf[size];
    ssize_t read_bytes;

    if ((fd = open("my_file", O_RDONLY)) <
        0) { //имя файла, что делаем с файлом, какие права предоставляем владельцу, группе и пользователю
        printf("Can\'t open file\n");
        exit(-1);
    }

    do{
        read_bytes = read(fd, buf, size);
        if (read_bytes == -1){
            printf("Can\'t write this file\n");
            exit(-1);
        }
        buf[read_bytes] = '\0';
        printf("%s\n", buf);
    } while (read_bytes == size);

    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }
    return 0;
}
#endif
