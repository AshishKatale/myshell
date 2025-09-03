#ifndef SHELL_H
#define SHELL_H 1

#define PATH_MAX_LEN 256
#define PROMPT_MAX_LEN PATH_MAX_LEN + 64

void shell_loop(void);
int  shell_cmd_exec(char *cmd_str);

#endif // !SHELL_H
