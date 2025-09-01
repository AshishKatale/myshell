#ifndef COMMAND_H
#define COMMAND_H 1

typedef struct {
  char *cmd;
  char **args;
  int nagrs;
  int args_cap;
} command;

typedef struct {
  command **cmds;
  int ncmds;
} cmd_pipeline;

int command_str_parse_and_exec(char *);
command *command_parse(char *, char **);
cmd_pipeline command_pipeline_parse(char *);

#endif // !COMMAND_H
