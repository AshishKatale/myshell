#ifndef PIPELINE_H
#define PIPELINE_H 1
#include "command.h"

struct pipeline {
  command *cmds;
  int ncmds;
  int ncmds_cap;
};

void pipeline_init();
void pipeline_reset();
void pipeline_free();
void pipeline_parse(char *, pipeline *);
int command_str_parse_and_exec(char *);

#endif // !PIPELINE_H
