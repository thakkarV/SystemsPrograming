#include "reader.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

char * read_input(bool * termiante)
{
	int buf_size = 512;
	char c;
	int index = 0;
	char * readbuf = calloc(buf_size, sizeof(char));

	while(true)
	{
		c = getchar();

		if (c == '\n' || c == EOF)
		{

			if (c == EOF)
				*termiante = true;

			break;
		}
		else
		{
			readbuf[index++] = c;
		}

		// handle reallocs
		if (index >= buf_size)
		{
			buf_size += buf_size;
			readbuf = realloc(readbuf, buf_size);

			if (!readbuf)
			{
				perror("realloc");
				exit(1);
			}
		}
	}

	readbuf[index + 1] = '\0';
	return readbuf;
}
