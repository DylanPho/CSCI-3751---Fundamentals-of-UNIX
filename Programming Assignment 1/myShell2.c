#include "apue.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGS 64  /* Initial max number of arguments (adjustable if needed)*/

static void sig_int(int); /* our signal-catching function */

int main(void) {
    char buf[MAXLINE];  /* from apue.h */
    char **cmd_strs;    /* array of strings */
    pid_t pid;
    int status;
   

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% "); /* print prompt (printf requires %% to print %) */
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; /* replace newline with null */

        /* allocate memory for cmd_strs dynamically */
        cmd_strs = malloc(MAXARGS * sizeof(char *));
        if (cmd_strs == NULL) {
            err_sys("malloc error");
            continue;
        }

        /* tokenizing input into arguments dynamically */
        int argc = 0;
        char *token = strtok(buf, " ");
        while (token != NULL) {
            cmd_strs[argc] = strdup(token); 
            if (cmd_strs[argc] == NULL) {
                err_sys("strdup error");
                exit(1);
            }
            argc++;
            if (argc >= MAXARGS - 1) 
                break;
            token = strtok(NULL, " ");
        }
        cmd_strs[argc] = NULL; 

        if (argc == 0) {
            printf("%% ");
            free(cmd_strs); 
            continue;
        }

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) { /* child process */
            execvp(cmd_strs[0], cmd_strs);
            err_ret("couldn't execute: %s", cmd_strs[0]);
            exit(127);
        }

        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");

        /* free allocated memory */
	int i;
        for (i = 0; i < argc; i++)
            free(cmd_strs[i]);
        free(cmd_strs);

        printf("%% ");
    }
    exit(0);
}

void sig_int(int signo) {
    printf("interrupt\n%% ");
}