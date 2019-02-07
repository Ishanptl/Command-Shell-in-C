# Variable for compiler
CC=gcc

# Variable for compiler options
CFLAGS=-c -g -std =c99
# Variable for cleanup command
# to clean .o file
CLEANUP=rm -rf *o shell

# Entry point and exit point for call to "make".
# Compile executable.
all:
	gcc shell.c -o shell

# Remove object
clean:
	$(CLEANUP)