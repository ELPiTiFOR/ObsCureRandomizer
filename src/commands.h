#ifndef COMMANDS_H
#define COMMANDS_H

void help();
void test();
void path(char *path);
void log_path(char *path);
void backup_path(char *path);
void backup_name_cmd(char *name);
int execute_commands(int argc, char **argv);
void script(char *path_script);

#endif // COMMANDS_H
