#ifndef SIGNAL_CONTROL_H
#define SIGNAL_CONTROL_H

void setup_shell_signal_handlers();
void setup_child_process_signal_behavior(pid_t child_pid);

#endif
