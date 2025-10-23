#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "pipeline.h"

static pipeline *cmd_pipe = NULL;
static pid_t *cmd_pids = NULL;
void pipeline_arena_init() {
  cmd_pipe = malloc(sizeof(pipeline));
  cmd_pipe->ncmds = 0;
  cmd_pipe->ncmds_cap = 3;
  cmd_pipe->command_running = 0;
  cmd_pipe->cmds = malloc(3 * sizeof(command));
}

void pipeline_arena_free() {
  free(cmd_pipe->cmds);
  free(cmd_pipe);
}

int pipeline_handle_sigint() {
  if (!cmd_pipe->command_running)
    return 0;

  for (int i = 0; i < cmd_pipe->ncmds; i++) {
    if (cmd_pids[i] > 0) {
      kill(cmd_pids[i], SIGTERM);
    }
  }

  // wait to clean up zombies
  for (int i = 0; i < cmd_pipe->ncmds; i++) {
    if (cmd_pids[i] > 0) {
      waitpid(cmd_pids[i], NULL, 0);
    }
  }

  cmd_pipe->command_running = 0;
  return cmd_pipe->ncmds;
}

void pipeline_arena_reset() {
  for (int i = 0; i < cmd_pipe->ncmds; ++i) {
    free(cmd_pipe->cmds[i].args);
  }
  cmd_pipe->ncmds = 0;
  cmd_pipe->command_running = 0;
}

void pipeline_cmd_print() {
  // printf("Pipe[%d/%d] [%p]:\n", cmd_pipe->ncmds, cmd_pipe->ncmds_cap,
  // cmd_pipe);
  printf("Pipe[%d]:\n", cmd_pipe->ncmds);
  for (int i = 0; i < cmd_pipe->ncmds; ++i) {
    printf("  ");
    command_print(&cmd_pipe->cmds[i]);
    printf("\n");
  }
}

int pipeline_cmd_parse(char *cmd_str) {
  char *delim = "|";
  char *cmd_str_token = strtok(cmd_str, delim);
  if (!cmd_str_token) {
    return 0;
  }

  while (cmd_str_token != NULL) {
    if (cmd_pipe->ncmds >= cmd_pipe->ncmds_cap) {
      cmd_pipe->ncmds_cap += 3;
      cmd_pipe->cmds =
          realloc(cmd_pipe->cmds, cmd_pipe->ncmds_cap * sizeof(command));
    }

    command cmd = command_parse(cmd_str_token);
    if (cmd.nagrs < 0) {
      return 1;
    }

    cmd_pipe->cmds[cmd_pipe->ncmds++] = cmd;
    cmd_str_token = strtok(NULL, delim);
  }

  return 0;
}

int pipeline_cmd_str_parse_and_exec(char *cmd_str) {
  int err = pipeline_cmd_parse(cmd_str);
  if (err) {
    return 1;
  }

  if (cmd_pipe->ncmds == 0) {
    return MYSH_CMD_EMPTY;
  }

  int pstatus = 0;
  int is_pipeline = cmd_pipe->ncmds > 1;
  cmd_pipe->command_running = 1;
  if (!is_pipeline) {
    command c = cmd_pipe->cmds[0];
    if (strcmp(c.cmd, "exit") == 0) {
      return MYSH_CMD_EXIT;
    } else if (strcmp(c.cmd, "cd") == 0) {
      return command_change_dir(&c);
    } else {
      pid_t pid =
          command_fork_and_exec(&c, STDIN_FILENO, STDOUT_FILENO, NULL, 0);
      cmd_pids = &pid;
      waitpid(pid, &pstatus, 0);
    }
  } else {
    int num_pipe = cmd_pipe->ncmds - 1;
    int num_fd = num_pipe * 2;
    int fd[num_fd];
    for (int i = 0; i < num_pipe; i++) {
      if (pipe(fd + i * 2) == -1) {
        perror("pipe creation failed");
        exit(1);
      }
    }

    pid_t pids[cmd_pipe->ncmds];
    cmd_pids = pids;
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
    pids[cmd_pipe->ncmds - 1] = command_fork_and_exec(
        &cmd_pipe->cmds[cmd_pipe->ncmds - 1],
        fd[(num_pipe - 1) * 2], // redirect last pipes read end to stdin
        STDOUT_FILENO, fd, num_fd);

    for (int i = 0; i < num_fd; i++) {
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
