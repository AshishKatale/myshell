#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

typedef struct {
  char *cmd;
  char **args;
  int nagrs;
  int args_cap;
} command;

command *command_parse(char *cmd_str) {
  command *cmd = malloc(sizeof(command));

  int nagrs = 0, args_cap = 4;
  char **args = malloc(args_cap * sizeof(char *));

  char *delim = " \t\r\n";
  char *token = strtok(cmd_str, delim);

  if (!token) {
    cmd->cmd = NULL;
    cmd->args = NULL;
    cmd->nagrs = 0;
    cmd->args_cap = args_cap;
    return cmd;
  }

  cmd->cmd = token;      // command name
  args[nagrs++] = token; // reuse command name as argv[0]

  while (token != NULL) {
    token = strtok(NULL, delim);
    if (nagrs >= args_cap) {
      args_cap += 4;
      args = realloc(args, args_cap * sizeof(char *));
    }
    args[nagrs++] = token;
  }
  args[nagrs] = NULL;

  cmd->nagrs = nagrs;
  cmd->args_cap = args_cap;
  cmd->args = args;

  return cmd;
}

int change_dir(command *cmd) {
  char *dir = cmd->args[1];
  if (!dir) {
    dir = getenv("HOME");
  }

  if (chdir(dir) == 0) {
    return 0;
  }

  fprintf(stderr, "%s: %s\n", dir, strerror(errno));
  return 1;
}

void shell_loop(void) {
  char *line;
  int exit_code = 0;

  while (1) {
    char *pwd = getcwd(NULL, 0);
    char prompt[strlen(pwd) + 10];
    sprintf(prompt, "%s [%d] $ ", pwd, exit_code);

    line = readline(prompt);
    if (line == NULL) {
      break;
    }

    if (line)
      add_history(line);

    command *cmd = command_parse(line);

    if (strcmp(cmd->cmd, "cd") == 0) {
      free(line);
      change_dir(cmd);
      continue;
    } else if (strcmp(cmd->cmd, "exit") == 0) {
      free(line);
      break;
    } else {
      printf("mysh: command not found: %s\n", cmd->cmd);
    }

    free(line);
  }

  clear_history();
}

int main(void) {
  shell_loop();
  return EXIT_SUCCESS;
}
