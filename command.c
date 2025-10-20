#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "shell.h"

void command_print(command *c) {
  if (!c)
    return;

  printf("Command: [%d]{ ", c->nagrs);
  for (int i = 0; i < c->nagrs; ++i) {
    printf("%s, ", c->args[i]);
  }
  printf("} ");
}

char *command_next_arg(char *str, char **saveptr, int *err) {
  char *p = str ? str : *saveptr;
  *err = 0;

  while (*p && (*p == ' ' || *p == '\t'))
    p++;
  if (!*p)
    return NULL;

  char *start;

  if (*p == '"' || *p == '\'') {
    char *quote = p;
    p++;
    start = p;
    while (*p && *p != *quote)
      p++;

    if (*p) {
      *p = '\0';
      p++;
    } else if (*p != *quote) {
      *err = 1;
      fprintf(stderr, "...%s\n   ^ ERROR: matching `%c` not found.\n", quote,
              *quote);
      return NULL;
    }
  } else {
    start = p;
    while (*p && *p != ' ' && *p != '\t')
      p++;
    if (*p) {
      *p = '\0';
      p++;
    }
  }

  *saveptr = p;
  return start;
}

command command_parse(char *cmd_str) {
  command cmd;
  char *save_ptr;
  int nagrs = 0, args_cap = 4;
  int err = 0;

  char **args = malloc(args_cap * sizeof(char *));
  char *cmd_token = command_next_arg(cmd_str, &save_ptr, &err);

  if (err) {
    cmd.nagrs = -1; // indicate error pasrsing args
    return cmd;
  }

  if (!cmd_token) {
    cmd.cmd = NULL;
    cmd.args = NULL;
    cmd.nagrs = 0;
    return cmd;
  }

  cmd.cmd = cmd_token;       // command name
  args[nagrs++] = cmd_token; // reuse command name as argv[0]

  while (!err && cmd_token != NULL) {
    cmd_token = command_next_arg(NULL, &save_ptr, &err);
    if (nagrs >= args_cap) {
      args_cap += 4;
      args = realloc(args, args_cap * sizeof(char *));
    }
    args[nagrs++] = cmd_token;
  }

  if (err) {
    cmd.nagrs = -1; // indicate error pasrsing args
    return cmd;
  }

  cmd.nagrs = nagrs;
  cmd.args = args;
  return cmd;
}

int command_change_dir(command *cmd) {
  char *dir_arg = cmd->args[1];
  char *home = getenv("HOME");
  char cdir[PATH_MAX_LEN];

  if (!dir_arg || strcmp(dir_arg, "~") == 0) {
    snprintf(cdir, sizeof(cdir), "%s", home);
  } else if (strncmp(dir_arg, "~/", 2) == 0) {
    snprintf(cdir, sizeof(cdir), "%s/%s", home, dir_arg + 2);
  } else {
    snprintf(cdir, sizeof(cdir), "%s", dir_arg);
  }

  if (chdir(cdir) == 0) {
    return 0;
  }

  fprintf(stderr, "%s: %s\n", dir_arg, strerror(errno));
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
