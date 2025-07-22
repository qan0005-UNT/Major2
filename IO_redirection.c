#include "IO_redirection.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int handle_redirection(char **args) {
    int i = 0;
    int in_fd = -1, out_fd = -1;
    char *in_file = NULL, *out_file = NULL;
    
    // Find redirection operators
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            // Input redirection
            if (args[i+1] == NULL) {
                fprintf(stderr, "syntax error: no input file specified\n");
                return -1;
            }
            in_file = args[i+1];
            args[i] = NULL; // Remove '<' from arguments
            args[i+1] = NULL; // Remove filename from arguments
            i += 2;
        } else if (strcmp(args[i], ">") == 0) {
            // Output redirection
            if (args[i+1] == NULL) {
                fprintf(stderr, "syntax error: no output file specified\n");
                return -1;
            }
            out_file = args[i+1];
            args[i] = NULL; // Remove '>' from arguments
            args[i+1] = NULL; // Remove filename from arguments
            i += 2;
        } else {
            i++;
        }
    }
    
    // Set up input redirection
    if (in_file != NULL) {
        in_fd = open(in_file, O_RDONLY);
        if (in_fd == -1) {
            perror("open input file");
            return -1;
        }
        if (dup2(in_fd, STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            close(in_fd);
            return -1;
        }
        close(in_fd);
    }
    
    // Set up output redirection
    if (out_file != NULL) {
        out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1) {
            perror("open output file");
            return -1;
        }
        if (dup2(out_fd, STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            close(out_fd);
            return -1;
        }
        close(out_fd);
    }
    
    return 0;
}