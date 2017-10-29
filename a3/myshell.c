#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#include "reader.h"
#include "parser.h"
#include "executor.h"
#include "datastructures.h"

void ignore_sig(int sig)
{
	printf("\n");
}

int main(int argc, char const * argv [])
{
	signal(SIGINT, ignore_sig);
	signal(SIGTERM, ignore_sig);

	char * input; // the line read from the terminal to be parsed and execed
	process ** exec_list; // parsed and tokenized input line converted to a list of processes to be execed
	bool terminate = false; // true if shell should exit

	// ideally we want to load this form a config file

	while (!terminate)
	{
		prompt();

		// read
		input = read_input(&terminate);
		printf("%s\n", input);

		// parse
		exec_list = parse(input);

		// execute
		execute(exec_list);

		// free up memory
		free(input);
		input = NULL;
		dealloc_exec_list(exec_list);
		free(exec_list);
		exec_list = NULL;
	}
	
	return EXIT_SUCCESS;
}

