CC = gcc
CFLAGS = -Wall -g
OBJ = main.o shell.o builtins.o utils.o external.o

default: minishell

minishell: $(OBJ)
	$(CC) -o minishell $(OBJ)

clean:
	rm -f *.o minishell
