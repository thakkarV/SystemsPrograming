CC = gcc

OBJS = myshell.o shell_funcs.o

.PHONY: all clean

all : myshell

myshell: $(OBJS)
	$(CC) $(OBJS) -o myshell

myshell.o: myshell.c
	$(CC) -c myshell.c

shell_funcs.o: shell_funcs.c shell_funcs.h
	$(CC) -c shell_funcs.c
	
clean:
	rm -rf $(OBJS)
