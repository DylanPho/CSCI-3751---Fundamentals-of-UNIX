#include "apue.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Re-implementation of dup2 without using fcntl */
int my_dup2_without_fcntl(int oldfd, int newfd) {
    if (oldfd == newfd) return newfd;

    if (close(newfd) < 0 && errno != EBADF) {
        perror("close failed");
        return -1;
    }

    int tmp_fds[1024];
    int i = 0;
    int fd;

    /* Duplicate until we get newfd */
    while ((fd = dup(oldfd)) != newfd) {
        if (fd < 0) {
            perror("dup failed");
            while (i > 0) close(tmp_fds[--i]);
            return -1;
        }
        tmp_fds[i++] = fd;
    }

    /* Cleanup intermediate FDs */
    while (i > 0) close(tmp_fds[--i]);
    return newfd;
}

void print_fd_path(int fd) {
    char path[256], buf[256];
    snprintf(path, sizeof(path), "/proc/self/fd/%d", fd);
    ssize_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        printf("%s -> %s\n", path, buf);
    } else {
        perror("readlink failed");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <new_fd> <file>\n", argv[0]);
        exit(1);
    }

    char *endptr;
    int newfd = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid new_fd: must be an integer\n");
        exit(1);
    }

    const char *filename = argv[2];

    int oldfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (oldfd < 0) {
        perror("open failed");
        exit(1);
    }

    int dupfd = my_dup2_without_fcntl(oldfd, newfd);
    if (dupfd < 0) {
        close(oldfd);
        exit(1);
    }

    printf("old fd: %d\nnew fd after my_dup2_without_fcntl: %d\n", oldfd, dupfd);
    print_fd_path(oldfd);
    print_fd_path(dupfd);

    if (dprintf(dupfd, "Hello from my_dup2_without_fcntl!\n") < 0) {
        perror("write failed");
    }

    close(oldfd);
    close(dupfd);
    return 0;
}
