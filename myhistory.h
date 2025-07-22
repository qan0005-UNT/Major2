#ifndef MYHISTORY_H
#define MYHISTORY_H

void init_history();
void add_history(const char *cmd);
void handle_myhistory(char **args, int argc);

#endif
