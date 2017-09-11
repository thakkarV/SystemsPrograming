#include <stdio.h>

int main(int argc, char const * argv [])
{
	bool newline_at_end = true;
	int counter = 1;

	if (argc > 1)
	{
		if (*argv[1] == "-n")
		{
			newline_at_end = false;
			counter = 2;
		}

	}
	for (int i = counter; i < argc; ++i)
	{
		printf(*argv[i]);
	}

	if (newline_at_end)
	{
		printf("\n");
	}

	return 0;
}
