CFLAGS = -Wall -Wextra -O3
LIBS = -lreadline
SRC = $(wildcard *.c)

myshell: $(SRC)
	$(CC) -o myshell $(CFLAGS) $(LIBS) $(SRC)

debug: $(SRC)
	$(CC)  -o myshell-dbg -ggdb $(LIBS) $(SRC)

.PHONY: run clean

run:
	./shell

clean:
	rm shell

