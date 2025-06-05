#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primeproc(int fd) {
    int prime;
    int n;
    int r = read(fd, &prime, sizeof(prime));
    if (r == 0) {
        exit(0);
    }
    printf("prime %d\n", prime);
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        primeproc(p[0]);
        close(p[0]);
        exit(0);
    } else {
        close(p[0]);
        while (read(fd, &n, sizeof(n)) > 0) {
            if (n % prime != 0) {
                write(p[1], &n, sizeof(n));
            }
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int n[33];
    for (int i = 0; i < 33; i++) {
        n[i] = i + 2;
    }
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        primeproc(p[0]);
        close(p[0]);
        exit(0);
    } else {
        close(p[0]);
        for (int i = 0; i < 33; i++) {
            write(p[1], &n[i], sizeof(int));
        }
        close(p[1]);
        wait(0);
    }

    exit(0);
}
