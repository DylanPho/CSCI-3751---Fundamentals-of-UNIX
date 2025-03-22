#include "apue.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <wordexp.h>
#include <unistd.h>

#define MAXARGS 64 /* Maximum number of arguments */
#define MAXCMDLEN 1024 /* Maximum input length */

static void sig_int(int); /* Signal handler function */

/* Function to handle output redirection (">") */
int redirect_output(char **cmd_args) {
    int i = 0;
    while (cmd_args[i] != NULL) {
        if (strcmp(cmd_args[i], ">") == 0) {
            /* Found redirection operator */
            if (cmd_args[i + 1] == NULL) {
                fprintf(stderr, "Error: No output file specified.\n");
                return -1;
            }
            int fd = open(cmd_args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("open");
                return -1;
            }
            dup2(fd, STDOUT_FILENO); /* Redirect stdout to file */
            close(fd);

            /* Remove '>' and filename from command args */
            cmd_args[i] = NULL;
            return 0;
        }
        i++;
    }
    return 0;
}

/* Function to expand wildcards (e.g., "ls she*") */
void expand_wildcards(char ***cmd_strs, int *argc) {
    wordexp_t p;
    if (wordexp((*cmd_strs)[0], &p, 0) == 0) {
        /* Free existing args */
        for (int i = 0; i < *argc; i++) {
            free((*cmd_strs)[i]);
        }
        free(*cmd_strs);

        /* Allocate new arguments */
        *cmd_strs = malloc(p.we_wordc * sizeof(char *));
        if (*cmd_strs == NULL) {
            perror("malloc");
            exit(1);
        }
        for (int i = 0; i < p.we_wordc; i++) {
            (*cmd_strs)[i] = strdup(p.we_wordv[i]);
        }
        *argc = p.we_wordc;
        (*cmd_strs)[*argc] = NULL;
        wordfree(&p);
    }
}

/* Function to parse input into an array of strings */
int parse_command(char *input, char ***cmd_strs) {
    char *token;
    int count = 0;

    /* Allocate memory for cmd_strs array */
    *cmd_strs = malloc(MAXARGS * sizeof(char *));
    if (*cmd_strs == NULL) {
        perror("malloc");
        exit(1);
    }

    /* Tokenize input using space as a delimiter */
    token = strtok(input, " ");
    while (token != NULL && count < MAXARGS - 1) {
        (*cmd_strs)[count] = strdup(token); /* Allocate memory for each token */
        token = strtok(NULL, " ");
        count++;
    }
    (*cmd_strs)[count] = NULL; /* Null-terminate argument list */
    
    return count; /* Return number of parsed arguments */
}

int main(void) {
    char buf[MAXCMDLEN]; /* Input buffer */
    char **cmd_args; /* Pointer for storing parsed arguments */
    pid_t pid;
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("%% "); /* Print shell prompt */

    while (fgets(buf, MAXCMDLEN, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; /* Replace newline with null terminator */

        /* Parse user input into array of strings */
        int argc = parse_command(buf, &cmd_args);

        if (argc == 0) { /* Skip empty commands */
            free(cmd_args);
            printf("%% ");
            continue;
        }

        /* Check for special parsing using strchr() */
        if (strchr(buf, '=') != NULL) {
            printf("Detected assignment operation, not executing.\n");
            free(cmd_args);
            printf("%% ");
            continue;
        }

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) { /* Child process */
            redirect_output(cmd_args); /* Handle output redirection */
            expand_wildcards(&cmd_args, &argc); /* Handle wildcard expansion */

            execvp(cmd_args[0], cmd_args);
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        /* Parent process waits for child */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");

        /* Free allocated memory */
        for (int i = 0; i < argc; i++) {
            free(cmd_args[i]);
        }
        free(cmd_args);

        printf("%% "); /* Print next prompt */
    }
    exit(0);
}

/* Signal handler for Ctrl+C */
void sig_int(int signo) {
    printf("interrupt\n%% ");
}

