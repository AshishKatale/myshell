#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "pipeline.h"

static pipeline *cmd_pipe = NULL;
void pipeline_init() {
  cmd_pipe = malloc(sizeof(pipeline));
  cmd_pipe->ncmds = 0;
  cmd_pipe->ncmds_cap = 3;
  cmd_pipe->cmds = malloc(3 * sizeof(command));
}

void pipeline_free() {
  free(cmd_pipe->cmds);
  free(cmd_pipe);
}

void pipeline_reset() {
  for (int i = 0; i < cmd_pipe->ncmds; ++i) {
    free(cmd_pipe->cmds[i].args);
  }
  cmd_pipe->ncmds = 0;
}

void pipeline_print() {
  // printf("Pipe[%d/%d] [%p]:\n", cmd_pipe->ncmds, cmd_pipe->ncmds_cap,
  // cmd_pipe);
  printf("Pipe[%d]:\n", cmd_pipe->ncmds);
  for (int i = 0; i < cmd_pipe->ncmds; ++i) {
    printf("  ");
    command_print(&cmd_pipe->cmds[i]);
    printf("\n");
  }
}

void pipeline_parse(char *cmd_str, pipeline *pipe) {
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

  pipeline_parse(cmd_str, cmd_pipe);
  if (cmd_pipe->ncmds == 0) {
    return 0;
  }

  int pstatus = 0;
  int is_pipeline = cmd_pipe->ncmds > 1;
  if (!is_pipeline) {
    command c = cmd_pipe->cmds[0];
    if (strcmp(c.cmd, "exit") == 0) {
      return -1;
    } else if (strcmp(c.cmd, "cd") == 0) {
      int ex = change_dir(&c);
      return ex;
    } else {
      pid_t pid =
          command_fork_and_exec(&c, STDIN_FILENO, STDOUT_FILENO, NULL, 0);
      waitpid(pid, &pstatus, 0);
    }
  } else {
    int num_fd = (cmd_pipe->ncmds - 1) * 2;
    int fd[num_fd];
    for (int i = 0; i < cmd_pipe->ncmds - 1; i++) {
      if (pipe(fd + i * 2) == -1) {
        perror("pipe");
        exit(1);
      }
    }

    pid_t pids[cmd_pipe->ncmds];

    // exec first command
    pids[0] =
        command_fork_and_exec(&cmd_pipe->cmds[0], STDIN_FILENO,
                              fd[1], // redirect stdout to first pipes write end
                              fd, num_fd);

    // exec middle commands
    for (int i = 1; i < cmd_pipe->ncmds - 1; ++i) {
      pids[i] = command_fork_and_exec(&cmd_pipe->cmds[i],
                                      fd[(i - 1) * 2], // prev pipes read end
                                      fd[i * 2 + 1],   // next pipes write end
                                      fd, num_fd);
    }

    // exec last command
    pids[cmd_pipe->ncmds - 1] =
        command_fork_and_exec(&cmd_pipe->cmds[cmd_pipe->ncmds - 1],
                              fd[(cmd_pipe->ncmds - 1) * 2 -
                                 2], // redirect last pipes write end to stdin
                              STDOUT_FILENO, fd, num_fd);

    for (int i = 0; i < 2 * (cmd_pipe->ncmds - 1); i++) {
      close(fd[i]);
    }

    for (int i = 0; i < cmd_pipe->ncmds; i++) {
      waitpid(pids[i], &pstatus, 0);
    }
  }

  if (WIFEXITED(pstatus)) {
    return WEXITSTATUS(pstatus); // return exit code of last command
  }

  return 0;
}
