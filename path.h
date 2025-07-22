#ifndef PATH_H
#define PATH_H

void handle_path_command(char *line);
char **get_path_list();
void free_path_list();
void init_path();

#endif
