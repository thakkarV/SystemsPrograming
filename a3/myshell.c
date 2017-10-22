#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "reader.h"
#include "parser.h"
#include "datastructures.h"

int main(int argc, char const * argv [])
{
	char * input; // the line read from the terminal to be parsed and execed
	process ** exec_list; // parsed and tokenized input line converted to a list of processes to be execed
	bool terminate = false; // true if shell should exit

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
	
	return EXIT_SUCCESS;
}
