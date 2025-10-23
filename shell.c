#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "pipeline.h"
#include "shell.h"

int shell_cmd_exec(char *cmd_str) {
  pipeline_arena_init();
  int exit_code = pipeline_cmd_str_parse_and_exec(cmd_str);
  pipeline_arena_reset();
  pipeline_arena_free();
  return exit_code;
}

void generate_prompt(char *prompt, int exit_code) {
  char *red = "\033[1;31m";
  char *green = "\033[0;32m";
  char *blue = "\033[1;34m";
  char *yellow = "\033[0;33m";
  char *reset = "\033[0m";

  char pwd[PATH_MAX_LEN];
  char shorten_pwd[PATH_MAX_LEN];
  char *home = getenv("HOME");

  if (!getcwd(pwd, PATH_MAX_LEN)) {
    strcpy(pwd, "???");
  }

  if (home && strncmp(pwd, home, strlen(home)) == 0) {
    snprintf(shorten_pwd, PATH_MAX_LEN, "~%s", pwd + strlen(home));
  } else {
    strcpy(shorten_pwd, pwd);
  }

  snprintf(prompt, PROMPT_MAX_LEN, "%s%s %s[%d] %s$%s ", blue, shorten_pwd,
           exit_code > 0 ? red : green, exit_code, yellow, reset);
}

void shell_loop(void) {
  const char *histfile = "/tmp/.mysh_history";
  read_history(histfile);
  pipeline_arena_init();

  char *line;
  char prompt[PROMPT_MAX_LEN];
  int exit_code = 0;
  while (1) {
    generate_prompt(prompt, exit_code);
    line = readline(prompt);
    if (line == NULL) {
      break;
    }

    if (*line && *line != ' ')
      add_history(line);

    int ex = pipeline_cmd_str_parse_and_exec(line);
    pipeline_arena_reset();

    if (ex == MYSH_CMD_EMPTY) {
      free(line);
      continue;
    }

    if (ex == MYSH_CMD_EXIT) {
      free(line);
      break;
    }

    exit_code = ex;
    free(line);
  }

  pipeline_arena_free();
  write_history(histfile);
  clear_history();
}
