### myshell
> - A simple interactive shell for educational purpose
> - Supports external commands and command pipelines
> - Shell builtins are not available except `cd` and `exit`.

```
$ ./mysh -h
mysh:
   -c <command>   execute the command
   -h             print this help message

Usage:
    ./mysh
    ./mysh -c 'echo hello world!'
```


```
$ ./mysh -c 'echo hello world!'
hello world!
$ ./mysh -c 'echo hello world! | base64'
aGVsbG8gd29ybGQhCg==
$ ./mysh -c 'echo hello world! | base64 | base64 -d'
hello world!
```

```
$ ./mysh
/home/user/projects/shell [0] $ echo hello world!
hello world!
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ cd ..
/home/user/projects [0] $ 
/home/user/projects [0] $ 
/home/user/projects [0] $ cd
/home/user [0] $ 
/home/user [0] $ 
/home/user [0] $ cd projects/shell/
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ 
/home/user/projects [0] $ cd ~
/home/user [0] $ 
/home/user [0] $ 
/home/user [0] $ cd projects/shell/
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ ls
command.c  command.h  main.c  Makefile  mysh  pipeline.c  pipeline.h  README.md  shell.c  shell.h
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ ls -l | grep c$
-rw-r--r-- 1 ashish ashish  2322 Sep  3 22:20 command.c
-rw-r--r-- 1 ashish ashish   580 Sep  3 16:59 main.c
-rw-r--r-- 1 ashish ashish  3382 Sep  3 17:22 pipeline.c
-rw-r--r-- 1 ashish ashish  1736 Sep  3 22:19 shell.c
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ ls -l | grep c$ | wc -l
4
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ unknown-command
mysh: command not found: unknown-command
/home/user/projects/shell [127] $ 
/home/user/projects/shell [127] $ 
/home/user/projects/shell [127] $ exit

```
