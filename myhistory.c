#include "myhistory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 20

static char *history[MAX_HISTORY];
static int current = 0;
static int total = 0;

void init_history() {
    for (int i = 0; i < MAX_HISTORY; ++i)
        history[i] = NULL;
}

void add_history(const char *cmd) {
    if (history[current]) free(history[current]);
    history[current] = strdup(cmd);
    current = (current + 1) % MAX_HISTORY;
    if (total < MAX_HISTORY) total++;
}

void handle_myhistory(char **args, int argc) {
    if (argc == 1) {
        for (int i = 0; i < total; ++i) {
            int index = (current - total + i + MAX_HISTORY) % MAX_HISTORY;
            printf("%d  %s\n", i, history[index]);
        }
    } else if (argc == 2 && strcmp(args[1], "-c") == 0) {
        for (int i = 0; i < MAX_HISTORY; ++i) {
            free(history[i]);
            history[i] = NULL;
        }
        current = total = 0;
    } else if (argc == 3 && strcmp(args[1], "-e") == 0) {
        int index = atoi(args[2]);
        if (index < 0 || index >= total) {
            fprintf(stderr, "myhistory: invalid history number\n");
            return;
        }
        int real_index = (current - total + index + MAX_HISTORY) % MAX_HISTORY;
        printf("Executing: %s\n", history[real_index]);
        extern void process_line_external(const char *line);
        process_line_external(history[real_index]);
    } else {
        fprintf(stderr, "myhistory: invalid arguments\n");
    }
}
