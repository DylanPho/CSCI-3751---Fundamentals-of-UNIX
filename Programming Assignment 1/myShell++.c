#include "apue.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXCMDLEN 1024 /* Maximum input length */

static void sig_int(int); /* Signal handler function */

int main(void) {
    char buf[MAXCMDLEN]; /* Input buffer */
    pid_t pid;
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% "); /* Print shell prompt */

    while (fgets(buf, MAXCMDLEN, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; /* Replace newline with null terminator */

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) { /* Child process */
            /* Execute command using Bash */
            execlp("bash", "bash", "-c", buf, NULL);
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        /* Parent process waits for child */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");

        printf("%% "); /* Print next prompt */
    }
    exit(0);
}

/* Signal handler for Ctrl+C */
void sig_int(int signo) {
    printf("interrupt\n%% ");
}
