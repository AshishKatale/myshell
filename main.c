#include <stdio.h>
#include <unistd.h>

#include "shell.h"

int main(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "hc:")) != -1) {
    switch (opt) {
    case 'h':
      printf("mysh:\n");
      printf("   -c <command>   execute the command\n");
      printf("   -h             print this help message\n\n");
      printf("Usage:\n");
      printf("    ./mysh\n");
      printf("    ./mysh -c 'echo hello world!'\n");
      return 0;
    case 'c':
      return shell_cmd_exec(optarg);
    default:
      return 1;
    }
  }
  shell_loop();
  return 0;
}
