#include "cd.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void builtin_cd(const char *path) {
    if (path == NULL || strcmp(path, "") == 0) {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }

    // attempt to update dir
    if (chdir(path) != 0) {
        perror("cd failed");
        return;
    }

    // Update the current working directory in the environment
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (setenv("PWD", cwd, 1) != 0) {
            perror("cd: setenv failed");
        }
    } else {
        perror("getcwd failed");
    }
}
