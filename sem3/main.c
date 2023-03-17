#define task1

#ifdef task1
#include <unistd.h>
#include <stdio.h>

int main() {
    /*uid_t uid = getuid();
    gid_t gid = getgid();

    printf("My uid = %d, my gid = %d\n", (int) uid, (int) gid);
    return 0;*/

    /*pid_t parent1 = getpid();
    pid_t parent2 = getppid();

    printf("My parent1 = %d, my parent2 = %d\n", (int) parent1, (int) parent2);*/

    pid_t pid, ppid, chpid;
    int a = 0;
    chpid = fork(); // создание второго потока

    a = a + 1;
    pid = getpid(); // получить id текущего процесса
    ppid = getppid(); // получить id родительского процесса
    printf("My pid = %d, my ppid = %d, child pid = %d, result = %d\n",
           (int) pid, (int) ppid, (int) chpid, a);
    printf("%p ", &a);

    if (chpid == 0){
        printf("I am child\n");
    }
    else{
        printf("I am parent\n");
    }
}
#endif

#ifdef task2
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    for (int i=0; envp[i] != NULL; ++i){
        printf("%s\n", envp[i]);
    }
    (void) execle("/bin/cat", "/bin/cat", "main.c", 0, envp);  // вывод всего кода
    printf("Error on program start\n"); // сюда попадаем только при возникновении ошибки
    exit(-1);
    return 0;
}
#endif