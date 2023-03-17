#include <unistd.h>
#include <stdio.h>

unsigned long long factorial(int n) {
    if (n == 1) {
        return n;
    }
    return factorial(n - 1) * n;
}

int fib(int n) {
    if (n == 1 || n == 2) {
        return 1;
    }
    if (n == 0) {
        return 0;
    }
    return fib(n - 1) + fib(n - 2);
}

int fib2(int n) {
    if (n < 2) {
        return n;
    }
    int arr[n + 1];
    arr[0] = 0;
    arr[1] = 1;
    for (int i = 2; i <= n; ++i) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }
    return arr[n];
}

int main() {
    pid_t chpid;
    int n;
    scanf("%d", &n);
    chpid = fork(); // создание второго потока

    if (chpid == 0) { // child
        printf("Factorial = %lld\n", factorial(n));
    } else { // parent
        printf("Number of Fibonacci(recursive) = %d\n", fib(n));
        printf("Number of Fibonacci(dynamic) = %d\n", fib2(n));
    }
}