#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "path.h"

#define MAX_PATHS 100

static char *path_list[MAX_PATHS];
static int path_count=0;

void print_path_list(){
    for (int i=0; i<path_count; i++){
        printf("%s", path_list[i]);
        if (i<path_count-1) printf(":");
    }
    printf("\n");
}

void add_path(const char *new_path){
    if (path_count<MAX_PATHS){
        path_list[path_count++]=strdup(new_path);
    }
}

void remove_path(const char *target_path){
    for (int i=0; i<path_count; i++){
        if (strcmp(path_list[i], target_path) ==0){
            free(path_list[i]);
            for (int j=i; j<path_count-1; j++){
                path_list[j]=path_list[j+1];
            }
            path_count--;
            break;
        }
    }
}

void handle_path_command(char *line){
    char *cmd=strtok(line, " \t\n");
    if(!cmd) return;

    char *arg=strtok(NULL, " \t\n");
    if(!arg){
        print_path_list();
        return;
    }

    if(strcmp(arg, "+") ==0){
        char *new_path=strtok(NULL, " \t\n");
        if (new_path) add_path(new_path);
        else fprintf(stderr, "path + requires a pathname\n");
    }
    else if (strcmp(arg, "-") ==0){
        char *rm_path=strtok(NULL, " \t\n");
        if (rm_path) remove_path(rm_path);
        else fprintf(stderr, "path - requires a pathname\n");  
    }
    else{
        fprintf(stderr, "Invalid path command format\n");
    }
}

char **get_path_list() {
    return path_list;
}

// Cleanup
void free_path_list() {
    for (int i = 0; i < path_count; i++) {
        free(path_list[i]);
    }
    path_count = 0;
}

void init_path() {
    path_count = 0;
}
