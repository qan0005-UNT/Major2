#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_exit.h"

static char *trim_whitespace(char *str) {
    while (*str == ' ' || *str == '\t') str++;
    if (*str == '\0') return str;

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n')) *end-- = '\0';
    return str;
}

int handle_exit_command(char *input_line) {
    char *copy = strdup(input_line);
    char *token = strtok(copy, ";");
    int should_exit = 0;

    while (token != NULL) {
        char *cmd = trim_whitespace(token);
        if (strncmp(cmd, "exit", 4) == 0 &&
            (cmd[4] == '\0' || cmd[4] == ' ' || cmd[4] == '\n')) {
            should_exit = 1;
        }
        token = strtok(NULL, ";");
    }

    free(copy);
    return should_exit;
}
