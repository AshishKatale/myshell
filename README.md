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
/home/user/projects/shell [0] $ cat /dev/random | base64 | head -n 10
U4bzPWK/rMiSYZnS3Cv4O/t2QDVyjbky4rIzttlALlPUdK/d04IEVZ1B5QkZky8nNflGkCxo3UFQ
xjPF1PFlKWbe4rlIkhdmZLZEaweOV52dOfth2LXnlwf7vZePX9OIAAPV69sQG8RB8UP5lJA5RR/t
vX49mfJBguCqk9vAH393kWYbgYvJh7zwLcgkRe1De2BQsSJ9Az5+USwbOHehPDp2kLXk/etKcrLt
JbhoMEV/HV7+z0xJ7pjCPL44uXMeHj+2GjBiSKPtjW3CU86tsXT1ViCy+K+uRgD06FGFBXK8vaTw
2zS67TD3pwJvVAOmzLfyGpFhedvIbz6M1EhueHJQvVtPE6u9Gg5ZQXNWTc5KtQ/icpRNYvBu1mA2
zFexq5IIbXuOFw422hv3fmUsUtxYs88IXn7lYxGj+xGaPbj05oj6kIRb8D5xY45Q4fL9FDpqpjCa
lHa+Fe4n1WawtMC1Wxb0XJw5u365jRlTYtv4dOnmDrEJijcN74tXg7ifhR0hyZSGFzXGtZ/gBvBg
poBmJGpzB89bXTC+ioAl8HebHELBRR8ZCnY5Pg/RQ0rkFup6Lx7gsmZPiiocI0J+6a8oJ85kNYwG
DkY5U12oPzeEQsYovM1phfKnbNRnsvTZLgWEQWCSgg4yFEejw4TZzbvw6oFw/0vaca8RKsJYRYo4
3mJB2vuByyjaYKdyRDd1FtHBB9Z+DmoL+PtnRIqvxKAWV5pKgPjd9YYByuuDsExf6kOmmBbdP6Nr
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ 
/home/user/projects/shell [0] $ unknown-command
mysh: command not found: unknown-command
/home/user/projects/shell [127] $ 
/home/user/projects/shell [127] $ 
/home/user/projects/shell [127] $ exit

```
