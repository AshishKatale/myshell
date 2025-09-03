#ifndef PIPELINE_H
#define PIPELINE_H 1
#include "command.h"

struct pipeline {
  command *cmds;
  int ncmds;
  int ncmds_cap;
};

void pipeline_arena_init(void);
void pipeline_arena_reset(void);
void pipeline_arena_free(void);
void pipeline_cmd_parse(char *cmd_str);
int  pipeline_cmd_str_parse_and_exec(char *cmd_str);

#endif // !PIPELINE_H
