#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2];
    int p2[2];
    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    int f = fork();
    if (f < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    if (f == 0) {
        close(p1[1]);
        close(p2[0]);
        char buf;
        if (read(p1[0], &buf, 1) != 1) {
            fprintf(2, "child: read error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        if (write(p2[1], &buf, 1) != 1) {
            fprintf(2, "child: write error\n");
            exit(1);
        }
        exit(0);
    }
    if (f > 0) {
        close(p1[0]);
        close(p2[1]);
        char buf = 'n';
        if (write(p1[1], &buf, 1) != 1) {
            fprintf(2, "parent: write error\n");
            exit(1);
        }
        if (read(p2[0], &buf, 1) != 1) {
            fprintf(2, "parent: read error\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        exit(0);
    }
    return 0;
}
