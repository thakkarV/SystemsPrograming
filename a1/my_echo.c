#include <stdio.h>
#include <string.h>

int main(int argc, char const * argv [])
{
	int newline_at_end = 1;
	int counter = argc;
	char * string = argv[0];
	char * newline_flag = "-n";
	char * space = " ";

	if (argc > 1)
	{
		string++;
		if (strcmp(string, newline_flag) == 0)
		{
			newline_at_end = 0;
			string++;
			counter--;
		}

		while (counter > 1)
		{
			printf(string++);
			counter--;

			if (counter > 0)
				printf(space);
		}
	}

	if (newline_at_end)
	{
		printf("\n");
	}

	return 0;
}
