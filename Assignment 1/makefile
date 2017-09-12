CC = gcc

my_echo: my_echo.o
	$(CC) my_echo.o -o my_echo

my_ehco.o: my_echo.c
	$(CC) -c my_echo.c

clean:
	rm -rf my_echo my_echo.o
