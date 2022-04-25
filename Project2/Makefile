CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread

all: proj2

# GENERATE .o FILES

proj2.o: proj2.c
	$(CC) $(CFLAGS) -c $< -o $@

# BUILD EXECUTABLES

proj2: proj2.o
	$(CC) $(CFLAGS) $< -o $@

# DELETE ALL .o FILES AND EXECUTABLE

clean:
	rm *.o proj2

win-clean:
	del *.o proj2.exe
