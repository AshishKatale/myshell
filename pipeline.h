#ifndef PIPELINE_H
#define PIPELINE_H 1
#include "command.h"

struct cmd_pipeline {
  command *cmds;
  int ncmds;
  int ncmds_cap;
};

void cmd_pipe_init();
void cmd_pipe_reset();
void cmd_pipe_free();
int command_str_parse_and_exec(char *);
void command_pipeline_parse(char *, cmd_pipeline *);

#endif // !PIPELINE_H
