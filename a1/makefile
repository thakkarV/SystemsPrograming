CC = gcc

.PHONY: all clean

all: my_echo

my_echo: my_echo.o
	$(CC) my_echo.o -o my_echo

my_ehco.o: my_echo.c
	$(CC) -c my_echo.c

clean:
	rm -f my_echo my_echo.o
