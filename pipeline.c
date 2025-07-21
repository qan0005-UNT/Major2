#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pipeline.h"

#define MAX_COMMANDS 3
#define MAX_ARGS 20

void execute_pipeline(char *line) {
    char *cmds[MAX_COMMANDS];
    int num_cmds = 0;

    char *token = strtok(line, "|");
    while (token != NULL && num_cmds < MAX_COMMANDS) {
        cmds[num_cmds++] = strdup(token);
        token = strtok(NULL, "|");
    }

    int pipefds[2 * (num_cmds - 1)];
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        char *argv[MAX_ARGS];
        int arg = 0;

        token = strtok(cmds[i], " \t\n");
        while (token != NULL && arg < MAX_ARGS - 1) {
            argv[arg++] = token;
            token = strtok(NULL, " \t\n");
        }
        argv[arg] = NULL;

        pid_t pid = fork();
        if (pid == 0) {
            if (i != 0)
                dup2(pipefds[(i - 1) * 2], 0);
            if (i != num_cmds - 1)
                dup2(pipefds[i * 2 + 1], 1);

            for (int j = 0; j < 2 * (num_cmds - 1); j++) close(pipefds[j]);

            execvp(argv[0], argv);
            perror("exec failed");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 2 * (num_cmds - 1); i++) close(pipefds[i]);

    for (int i = 0; i < num_cmds; i++) wait(NULL);

    for (int i = 0; i < num_cmds; i++) free(cmds[i]);
}
