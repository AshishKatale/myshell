#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "pipeline.h"

void shell_loop(void) {
  int exit_code = 0;
  char *red = "\033[1;31m";
  char *green = "\033[0;32m";
  char *blue = "\033[1;34m";
  char *yellow = "\033[0;33m";
  char *reset = "\033[0m";

  const char *histfile = ".mysh_history";
  read_history(histfile);
  pipeline_arena_init();

  char *line;
  while (1) {
    char *pwd = getcwd(NULL, 0);
    char prompt[strlen(pwd) + 32];

    sprintf(prompt, "%s%s %s[%d] %s$%s ", blue, pwd,
            exit_code > 0 ? red : green, exit_code, yellow, reset);
    free(pwd);

    line = readline(prompt);
    if (line == NULL) {
      break;
    }

    if (*line)
      add_history(line);

    exit_code = pipeline_cmd_str_parse_and_exec(line);
    pipeline_arena_reset();

    if (exit_code < 0) {
      free(line);
      break;
    }

    free(line);
  }

  pipeline_arena_free();
  write_history(histfile);
  clear_history();
}
