#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(char *argv[]) {
    char buf[512];
    char *args[MAXARG];
    int i;

    for (i = 0; argv[i + 1] != 0 && i < MAXARG - 1; i++) {
        args[i] = argv[i + 1];
    }

    int n = 0;
    char c;

    while (read(0, &c, 1) == 1) {
        if (c == '\n') {
            buf[n] = 0;

            int j = i;
            char *p = buf;
            while (*p && j < MAXARG - 1) {
                while (*p == ' ') p++;
                if (*p == 0) break;
                args[j++] = p;
                while (*p && *p != ' ') p++;
                if (*p) *p++ = 0;
            }
            args[j] = 0;

            if (fork() == 0) {
                exec(argv[1], args);
                fprintf(2, "xargs: exec failed\n");
                exit(1);
            }
            wait(0);
            n = 0;
        } else {
            if (n < sizeof(buf) - 1) {
                buf[n++] = c;
            }
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs <command>\n");
        exit(1);
    }
    xargs(argv);
    exit(0);
}