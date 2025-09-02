CFLAGS = -Wall -Wextra -O3
LIBS = -lreadline
SRC = $(wildcard *.c)

mysh: $(SRC)
	$(CC) -o mysh $(CFLAGS) $(LIBS) $(SRC)

debug: $(SRC)
	$(CC) -o mysh-dbg -ggdb $(LIBS) $(SRC)

.PHONY: run clean

run:
	./mysh

clean:
	rm mysh mysh-dbg

