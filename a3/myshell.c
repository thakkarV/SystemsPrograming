#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "reader.h"

int main(int argc, char const * argv [])
{
	char * input;
	char ** tokens;
	bool terminate = false;
	const char * prompt = "myshell$> ";

	while (!terminate)
	{
		printf(prompt);

		input = read_input(terminate);

		printf(input);

		

		// command = read_commands();

		free(input);
	}
	return 0;
}
