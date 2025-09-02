#ifndef COMMAND_H
#define COMMAND_H 1
#include <sys/types.h>

typedef struct pipeline pipeline;

typedef struct {
  char *cmd;
  char **args;
  int nagrs;
} command;

void command_print(command *);
command command_parse(char *cmd_str, char **strtok_save_ptr);
int change_dir(command *cmd);
pid_t command_fork_and_exec(command *c, int fdin, int fdout, int *fds_to_close,
                            int num_fd);

#endif // !COMMAND_H
