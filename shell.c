#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "command.h"

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

    if (strcmp(cmd->cmd, "exit") == 0) {
      free(line);
      break;
    } else {
      command_exec(cmd);
    }

    free(line);
  }

  clear_history();
}
