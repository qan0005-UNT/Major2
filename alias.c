#include "alias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALIASES 50

typedef struct {
    char name[64];
    char command[256];
} Alias;

static Alias aliases[MAX_ALIASES];
static int alias_count = 0;

void init_alias() {
    alias_count = 0;
}

void handle_alias(char **args, int argc) {
    if (argc == 1) {
        for (int i = 0; i < alias_count; ++i)
            printf("alias %s='%s'\n", aliases[i].name, aliases[i].command);
    } else if (argc == 2 && strcmp(args[1], "-c") == 0) {
        alias_count = 0;
    } else if (argc == 3 && strcmp(args[1], "-r") == 0) {
        for (int i = 0; i < alias_count; ++i) {
            if (strcmp(aliases[i].name, args[2]) == 0) {
                for (int j = i; j < alias_count - 1; ++j)
                    aliases[j] = aliases[j+1];
                alias_count--;
                return;
            }
        }
        fprintf(stderr, "alias: alias not found\n");
    } else if (argc == 2) {
        char *eq = strchr(args[1], '=');
        if (!eq || eq == args[1] || eq[1] != '\'') {
            fprintf(stderr, "alias: invalid format\n");
            return;
        }
        char name[64];
        char command[256];
        strncpy(name, args[1], eq - args[1]);
        name[eq - args[1]] = '\0';
        strncpy(command, eq + 2, strlen(eq + 2) - 1);
        command[strlen(eq + 2) - 1] = '\0';

        for (int i = 0; i < alias_count; ++i) {
            if (strcmp(aliases[i].name, name) == 0) {
                strncpy(aliases[i].command, command, 256);
                return;
            }
        }
        if (alias_count < MAX_ALIASES) {
            strncpy(aliases[alias_count].name, name, 64);
            strncpy(aliases[alias_count].command, command, 256);
            alias_count++;
        }
    } else {
        fprintf(stderr, "alias: invalid arguments\n");
    }
}

void alias_substitute(char *line) {
    char *first_token = strtok(line, " \t\n");
    if (!first_token) return;

    for (int i = 0; i < alias_count; ++i) {
        if (strcmp(first_token, aliases[i].name) == 0) {
            char new_line[512];
            snprintf(new_line, sizeof(new_line), "%s%s",
                     aliases[i].command,
                     line + strlen(first_token));
            strcpy(line, new_line);
            return;
        }
    }
}
