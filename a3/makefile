CC = gcc

OBJS = myshell.o reader.o parser.o

.PHONY: all clean

all : myshell

myshell: $(OBJS)
	$(CC) $(OBJS) -o myshell

myshell.o: myshell.c datastructures.h reader.h parser.h
	$(CC) -c myshell.c

reader.o: reader.c reader.h
	$(CC) -c reader.c

parser.o: parser.c parser.h datastructures.h
	$(CC) -c parser.c
	
clean:
	rm -rf myshell $(OBJS)
