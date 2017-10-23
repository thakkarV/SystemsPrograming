DEBUG ?= 1
ifeq (DEBUG, 1)
	CFLAGS = -g -c
else
	CLFAGS = -c
endif

CC = gcc
OBJS = myshell.o reader.o parser.o

.PHONY: all clean

all : myshell

myshell: $(OBJS)
	$(CC) $(OBJS) -o myshell

myshell.o: myshell.c datastructures.h reader.h parser.h
	$(CC) $(CFLAGS) myshell.c

reader.o: reader.c reader.h
	$(CC) $(CFLAGS) reader.c

parser.o: parser.c parser.h datastructures.h
	$(CC) $(CFLAGS) parser.c
	
clean:
	rm -rf myshell $(OBJS)
