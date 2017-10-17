#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "reader.h"
#include "datastructures.h"

int main(int argc, char const * argv [])
{
	char * input;
	process * exec_list;
	bool terminate = false;

	// ideally we want to load this form a config file
	const char * prompt = "myshell$> ";

	while (!terminate)
	{
		printf(prompt);

		// read
		input = read_input(&terminate);
		printf(input);

		// parse
		exec_list = parse(input);

		// execute
		// execute(exec_list);

		// free up memory
		free(input);
		free(exec_list);
	}
	return 0;
}
