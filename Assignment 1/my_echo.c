#include <stdio.h>
#include <string.h>

int main(int argc, char const * argv [])
{
	int newline_at_end = 1;
	int start = 1;
	char * newline_flag = "-n";
	char * space = " ";

	if (argc > 1)
	{
		if (strcmp((argv[1]), newline_flag) == 0)
		{
			newline_at_end = 0;
			start++;
		}

		while (start < argc - 1)
		{
			printf(argv[start]);
			printf(space);
			start++;
		}

		printf(argv[argc - 1]);
	}

	if (newline_at_end)
	{
		printf("\n");
	}

	return 0;
}
