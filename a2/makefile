CC = gcc

.PHONY: all clean

OBJS = finds.o

all: finds

finds: $(OBJS)
	$(CC) $(OBJS) -o finds

finds.o: finds.c
	$(CC) -c finds.c
	
clean:
	rm -f finds finds.o
