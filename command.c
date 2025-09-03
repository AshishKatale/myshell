#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"

void command_print(command *c) {
  if (!c)
    return;

  printf("Command: [%d]{ ", c->nagrs);
  for (int i = 0; i < c->nagrs; ++i) {
    printf("%s, ", c->args[i]);
  }
  printf("} ");
}

command command_parse(char *cmd_str, char **save_ptr) {
  command cmd;

  int nagrs = 0, args_cap = 4;
  char **args = malloc(args_cap * sizeof(char *));

  char *delim = " \t\r\n";
  char *cmd_token = strtok_r(cmd_str, delim, save_ptr);

  if (!cmd_token) {
    cmd.cmd = NULL;
    cmd.args = NULL;
    cmd.nagrs = 0;
    return cmd;
  }

  cmd.cmd = cmd_token;       // command name
  args[nagrs++] = cmd_token; // reuse command name as argv[0]

  while (cmd_token != NULL) {
    cmd_token = strtok_r(NULL, delim, save_ptr);
    if (nagrs >= args_cap) {
      args_cap += 4;
      args = realloc(args, args_cap * sizeof(char *));
    }
    args[nagrs++] = cmd_token;
  }

  cmd.nagrs = nagrs;
  cmd.args = args;
  return cmd;
}

int change_dir(command *cmd) {
  char *dir = cmd->args[1];
  if (!dir || strcmp(dir, "~") == 0) {
    dir = getenv("HOME");
  }

  if (chdir(dir) == 0) {
    return 0;
  }

  fprintf(stderr, "%s: %s\n", dir, strerror(errno));
  return 1;
}

pid_t command_fork_and_exec(command *c, int fdin, int fdout, int *fds_to_close,
                            int num_fd) {
  int pid = fork();
  if (pid == -1) {
    perror("mysh: fork failed");
    exit(1);
  }

  if (pid == 0) {
    // Child process
    if (fdin != STDIN_FILENO) {
      if (dup2(fdin, STDIN_FILENO) == -1) {
        perror("dup2 stdin failed");
        _exit(1);
      }
    }
    if (fdout != STDOUT_FILENO) {
      if (dup2(fdout, STDOUT_FILENO) == -1) {
        perror("dup2 stdout failed");
        _exit(1);
      }
    }

    for (int i = 0; i < num_fd; ++i) {
      close(fds_to_close[i]); // close all fds
    }

    execvp(c->cmd, c->args);

    // Only reached if execvp fails
    if (errno == ENOENT) {
      fprintf(stderr, "mysh: command not found: %s\n", c->cmd);
    } else if (errno == EACCES) {
      fprintf(stderr, "mysh: permission denied: %s\n", c->cmd);
    } else {
      fprintf(stderr, "mysh: %s: %s\n", c->cmd, strerror(errno));
    }

    _exit(127);
  }

  return pid;
}
