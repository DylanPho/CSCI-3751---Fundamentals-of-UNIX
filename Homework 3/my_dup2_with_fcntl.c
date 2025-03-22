#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int my_dup2_with_fcntl(int oldfd, int newfd) {
    if (oldfd == newfd) return newfd;

    close(newfd);  /* ensure it's closed */
    int dup_fd = fcntl(oldfd, F_DUPFD, newfd);
    if (dup_fd < 0) {
        perror("fcntl failed");
        return -1;
    }
    return dup_fd;
}

void print_fd_path(int fd) {
    char path[256], buf[256];
    snprintf(path, sizeof(path), "/proc/self/fd/%d", fd);
    ssize_t len = readlink(path, buf, sizeof(buf)-1);
    if (len != -1) {
        buf[len] = '\0';
        printf("%s: new fd file path: %s\n", path, buf);
    } else {
        perror("readlink failed");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <new_fd> <file>\n", argv[0]);
        exit(1);
    }

    int newfd = atoi(argv[1]);
    const char *filename = argv[2];

    int oldfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (oldfd < 0) {
        perror("open failed");
        exit(1);
    }

    int dupfd = my_dup2_with_fcntl(oldfd, newfd);
    if (dupfd < 0) exit(1);

    printf("old fd: %d\nnew duped fd as requested: %d\n", oldfd, dupfd);
    print_fd_path(oldfd);
    print_fd_path(dupfd);

    close(oldfd);
    close(dupfd);
    return 0;
}

