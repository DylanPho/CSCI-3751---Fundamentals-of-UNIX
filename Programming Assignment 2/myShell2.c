#include "apue.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAXARGS 64

static void sig_int(int); /* our signal-catching function */

int main(void) {
    char buf[MAXLINE];
    char **cmd_strs;
    pid_t pid;
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;

        cmd_strs = malloc(MAXARGS * sizeof(char *));
        if (cmd_strs == NULL) {
            err_sys("malloc error");
            continue;
        }

        int argc = 0;
        char *token = strtok(buf, " ");
        while (token != NULL) {
            cmd_strs[argc] = strdup(token); 
            if (cmd_strs[argc] == NULL) {
                err_sys("strdup error");
                exit(1);
            }
            argc++;
            if (argc >= MAXARGS - 1) break;
            token = strtok(NULL, " ");
        }
        cmd_strs[argc] = NULL;

        if (argc == 0) {
            printf("%% ");
            free(cmd_strs); 
            continue;
        }

        // Redirection flags and tracking
        int in_redirect = -1, out_redirect = -1, err_redirect = -1;
        for (int i = 0; i < argc; ++i) {
            if (strcmp(cmd_strs[i], ">") == 0 && i + 1 < argc)
                out_redirect = i;
            else if (strcmp(cmd_strs[i], "<") == 0 && i + 1 < argc)
                in_redirect = i;
            else if (strcmp(cmd_strs[i], ">&") == 0 && i + 1 < argc)
                err_redirect = i;
        }

        // Remove redirection tokens
        if (out_redirect != -1 || in_redirect != -1 || err_redirect != -1) {
            for (int j = 0; j < argc; ++j) {
                if (j == out_redirect || j == in_redirect || j == err_redirect) {
                    free(cmd_strs[j]);
                    free(cmd_strs[j + 1]);
                    for (int k = j; k + 2 <= argc; ++k)
                        cmd_strs[k] = cmd_strs[k + 2];
                    argc -= 2;
                    j = -1; // restart loop
                }
            }
            cmd_strs[argc] = NULL;
        }

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) { // child
            if (out_redirect != -1) {
                int fd = open(cmd_strs[out_redirect + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) err_sys("open error for output");
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            if (in_redirect != -1) {
                int fd = open(cmd_strs[in_redirect + 1], O_RDONLY);
                if (fd < 0) err_sys("open error for input");
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (err_redirect != -1) {
                int fd = open(cmd_strs[err_redirect + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) err_sys("open error for stderr");
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);
            }

            execvp(cmd_strs[0], cmd_strs);
            err_ret("couldn't execute: %s", cmd_strs[0]);
            exit(127);
        }

        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");

        for (int i = 0; i < argc; i++)
            free(cmd_strs[i]);
        free(cmd_strs);

        printf("%% ");
    }
    exit(0);
}

void sig_int(int signo) {
    printf("interrupt\n%% ");
}

