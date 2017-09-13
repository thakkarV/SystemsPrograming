#include <stdio.h>
#include <string.h>

int main(int argc, char const * argv [])
{
	int newline_at_end = 1;
	char * newline_flag = "-n";
	char * space = " ";
	int boundry = 1;

	if (argc > 1)
	{
		argv++;
		if (strcmp(argv, newline_flag) == 0)
		{
			newline_at_end = 0;
			*argv++;
			argc--;
			boundry = 2;
		}

		while (argc > boundry)
		{
			printf(*argv++);
			argc--;

			if (argc > boundry)
				printf(space);
		}
	}

	if (newline_at_end)
	{
		printf("\n");
	}

	return 0;
}
