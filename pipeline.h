#ifndef PIPELINE_H
#define PIPELINE_H 1
#include "command.h"

#define MYSH_CMD_EXIT -1
#define MYSH_CMD_EMPTY -2

struct pipeline {
  command *cmds;
  int ncmds;
  int ncmds_cap;
  int command_running;
};

void pipeline_arena_init(void);
void pipeline_arena_reset(void);
void pipeline_arena_free(void);
int pipeline_handle_sigint();
int pipeline_cmd_parse(char *cmd_str);
int pipeline_cmd_str_parse_and_exec(char *cmd_str);

#endif // !PIPELINE_H
