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
~/projects/shell [0] $ echo hello world!
hello world!
~/projects/shell [0] $ 
~/projects/shell [0] $ 
~/projects/shell [0] $ cd ..
~/projects [0] $ 
~/projects [0] $ 
~/projects [0] $ cd
~ [0] $ 
~ [0] $ 
~ [0] $ cd projects/shell/
~/projects/shell [0] $ 
~/projects/shell [0] $ 
~/projects [0] $ cd ~
~ [0] $ 
~ [0] $ 
~ [0] $ cd projects/shell/
~/projects/shell [0] $ 
~/projects/shell [0] $ 
~/projects/shell [0] $ ls
command.c  command.h  main.c  Makefile  mysh  pipeline.c  pipeline.h  README.md  shell.c  shell.h
~/projects/shell [0] $ 
~/projects/shell [0] $ 
~/projects/shell [0] $ ls -l | grep c$
-rw-r--r-- 1 user user  2322 Sep  3 22:20 command.c
-rw-r--r-- 1 user user   580 Sep  3 16:59 main.c
-rw-r--r-- 1 user user  3382 Sep  3 17:22 pipeline.c
-rw-r--r-- 1 user user  1736 Sep  3 22:19 shell.c
~/projects/shell [0] $ 
~/projects/shell [0] $ 
~/projects/shell [0] $ ls -l | grep c$ | wc -l
4
~/projects/shell [0] $ 
~/projects/shell [0] $ 
~/projects/shell [0] $ unknown-command
mysh: command not found: unknown-command
~/projects/shell [127] $ 
~/projects/shell [127] $ 
~/projects/shell [127] $ exit

```
