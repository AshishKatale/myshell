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

    exit_code = command_str_parse_and_exec(line);
    if (exit_code < 0)
      break;

    free(line);
  }

  clear_history();
}
