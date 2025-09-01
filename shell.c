#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

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

    free(line);
  }

  clear_history();
}

int main(void) {
  shell_loop();
  return EXIT_SUCCESS;
}
