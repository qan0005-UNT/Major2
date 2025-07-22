#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/wait.h>
#include "signal_control.h"

static pid_t shell_pgid;
static struct termios shell_tmodes;

void setup_shell_signal_handlers() {
    signal(SIGINT, SIG_IGN);   // Ctrl+C
    signal(SIGTSTP, SIG_IGN);  // Ctrl+Z

    shell_pgid = getpid();
    tcgetattr(STDIN_FILENO, &shell_tmodes);

    if (setpgid(shell_pgid, shell_pgid) < 0) {
        perror("Couldn't put shell in its own process group");
        exit(1);
    }
    tcsetpgrp(STDIN_FILENO, shell_pgid);
}

void setup_child_process_signal_behavior(pid_t child_pid) {
    if (setpgid(child_pid, child_pid) < 0) {
        perror("setpgid failed");
        exit(1);
    }

    tcsetpgrp(STDIN_FILENO, child_pid);

    int status;
    waitpid(child_pid, &status, 0);

    tcsetpgrp(STDIN_FILENO, shell_pgid);
}
