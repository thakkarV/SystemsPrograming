#include "reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char * read_input(void)
{
	int buf_size = 64;
	char c;
	int index = 0;
	char * readbuf = calloc(buf_size, sizeof(char));

	while(true)
	{
		c = getchar();

		if (c == '\n' || c == EOF)
		{
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
