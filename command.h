#ifndef COMMAND_H
#define COMMAND_H 1
#include <sys/types.h>

typedef struct cmd_pipeline cmd_pipeline;

typedef struct {
  char *cmd;
  char **args;
  int nagrs;
} command;

void command_print(command *);
command command_parse(char *, char **);
int change_dir(command *cmd);
pid_t command_fork_and_exec(command *, int, int, int);

#endif // !COMMAND_H
