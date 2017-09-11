CC = gcc

ehco: my_echo.c
	$(CC) -c my_echo.c -c my_echo

clean:
	rm -rf my_echo my_echo.o
