CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread

all: proj2

run: proj2
	./proj2 6 11 0 0
	cat proj2.out
	./proj2 10 21 0 0
	cat proj2.out

# GENERATE .o FILES

proj2.o: proj2.c
	$(CC) $(CFLAGS) -c $< -o $@

# BUILD EXECUTABLES

proj2: proj2.o
	$(CC) $(CFLAGS) $< -o $@

# DELETE ALL .o FILES AND EXECUTABLE

clean:
	rm *.o *.out* proj2