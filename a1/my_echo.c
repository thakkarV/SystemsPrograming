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
		*argv++;
		argc--;
		if (strcmp(argv[0], newline_flag) == 0)
		{
			printf(argv[0]);
			newline_at_end = 0;
			if (argc > 2 && !newline_at_end)
			{
				*argv++;
				argc--;
			}
		}

		while (argc > 0)
		{
			printf(*argv);
			argv++;
			argc--;

			if (argc > 0)
				printf(space);
		}
	}

	if (newline_at_end)
	{
		printf("\n");
	}

	return 0;
}
