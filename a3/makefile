CC = gcc

OBJS = myshell.o reader.o parser.o

.PHONY: all clean

all : myshell

myshell: $(OBJS)
	$(CC) $(OBJS) -o myshell

myshell.o: myshell.c
	$(CC) -c myshell.c

reader.o: reader.c reader.h
	$(CC) -c reader.c

parser.o: parser.c parser.h
	$(CC) -c parser.c
	
clean:
	rm -rf $(OBJS)
