# Compiler and compilation flags
CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

# The final executable target
symnmf: symnmf.o
	$(CC) $(CFLAGS) -o symnmf symnmf.o -lm

# Compile the object file
symnmf.o: symnmf.c symnmf.h
	$(CC) $(CFLAGS) -c symnmf.c

# Clean command to remove compiled files
clean:
	rm -f *.o symnmf