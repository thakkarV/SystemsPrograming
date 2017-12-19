#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>

#include "reader.h"
#include "parser.h"
#include "executor.h"

bool is_loaded;
bool is_running;
bool terminate;
char * elf_path = "";
pid_t child_pid;
breakpoint * bp_list_head;

void ignore_int(int);
void print_prompt(void);

int main(int argc, char const * argv [])
{
	// register the signal handler first for interrupt
	struct sigaction default_action;
	struct sigaction int_action;
	memset(&int_action, 0, sizeof(sigaction));
	int_action.sa_handler = ignore_int;
	if (sigaction(SIGINT, &int_action, &default_action) < 0)
		perror("sigaction");

	// now start debugger
	is_loaded = false;
	is_running = false;
	terminate  = false;
	while (!terminate)
	{
		print_prompt();
		char * input = read_input();
		printf("DBG : INPUT STR = \"%s\"\n", input);

		if (strcmp(input, "\n") != 0 &&
			strcmp(input, "") != 0)
		{
			int num_args;
			char ** tokens = tokenize(input, &num_args);
			printf("DBG : tokenise done. %d = num_args\n", num_args); 

			execute(tokens, num_args);
		}

		free(input);
	}

	return 0;
}

void ignore_int(int throwaway) { }

void print_prompt(void)
{
	printf("(mygdb) ");
}
