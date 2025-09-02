#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipeline.h"

static cmd_pipeline *cmd_pipe = NULL;
void cmd_pipe_init() {
  cmd_pipe = malloc(sizeof(cmd_pipeline));
  cmd_pipe->ncmds = 0;
  cmd_pipe->ncmds_cap = 3;
  cmd_pipe->cmds = malloc(3 * sizeof(command));
}

void cmd_pipe_free() {
  free(cmd_pipe->cmds);
  free(cmd_pipe);
}

void cmd_pipe_reset() {
  for (int i = 0; i < cmd_pipe->ncmds; ++i) {
    free(cmd_pipe->cmds[i].args);
  }
  cmd_pipe->ncmds = 0;
}

void command_pipeline_print() {
  printf("Pipe[%d/%d] [%p]:\n", cmd_pipe->ncmds, cmd_pipe->ncmds_cap, cmd_pipe);
  // printf("Pipe[%d]:\n", cmd_pipe->ncmds);
  for (int i = 0; i < cmd_pipe->ncmds; ++i) {
    printf("  ");
    command_print(&cmd_pipe->cmds[i]);
    printf("\n");
  }
}

void command_pipeline_parse(char *cmd_str, cmd_pipeline *pipe) {
  pipe->ncmds = 0;

  char *delim = "|";
  char *save_pipe = NULL, *save_cmd = NULL;
  char *cmd_str_token = strtok_r(cmd_str, delim, &save_pipe);
  if (!cmd_str_token) {
    return;
  }

  while (cmd_str_token != NULL) {
    if (pipe->ncmds >= pipe->ncmds_cap) {
      pipe->ncmds_cap += 3;
      pipe->cmds = realloc(pipe->cmds, pipe->ncmds_cap * sizeof(command));
    }
    pipe->cmds[pipe->ncmds++] = command_parse(cmd_str_token, &save_cmd);
    cmd_str_token = strtok_r(NULL, delim, &save_pipe);
  }

  return;
}

int command_str_parse_and_exec(char *cmd_str) {

  command_pipeline_parse(cmd_str, cmd_pipe);
  if (cmd_pipe->ncmds == 0) {
    return 0;
  }

  int is_pipeline = cmd_pipe->ncmds > 1;
  if (!is_pipeline) {
    command c = cmd_pipe->cmds[0];
    if (strcmp(c.cmd, "exit") == 0) {
      return -1;
    } else if (strcmp(c.cmd, "cd") == 0) {
      int ex = change_dir(&c);
      return ex;
    } else {
      int pstatus;
      pid_t pid = command_fork_and_exec(&c, STDIN_FILENO, STDOUT_FILENO, -1);
      waitpid(pid, &pstatus, 0);

      if (WIFEXITED(pstatus)) {
        return WEXITSTATUS(pstatus);
      }
    }
  } else {
    int fd[2];
    if (pipe(fd) == -1) {
      perror("pipe");
      exit(1);
    }
    // set stdout to pipe for cmd1
    pid_t pid1 =
        command_fork_and_exec(&cmd_pipe->cmds[0], STDIN_FILENO, fd[1], fd[0]);
    close(fd[1]);

    // set stdin to pipe for cmd2
    pid_t pid2 =
        command_fork_and_exec(&cmd_pipe->cmds[1], fd[0], STDOUT_FILENO, fd[1]);
    close(fd[0]);

    int pstatus;
    waitpid(pid1, &pstatus, 0);
    waitpid(pid2, &pstatus, 0);
  }

  return 0;
}
