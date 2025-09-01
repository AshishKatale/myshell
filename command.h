#ifndef COMMAND_H
#define COMMAND_H 1

typedef struct {
  char *cmd;
  char **args;
  int nagrs;
  int args_cap;
} command;

command *command_parse(char *);
int command_exec(command *);

#endif // !COMMAND_H
