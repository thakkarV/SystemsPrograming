// C utils
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// Sys utils
#include <signal.h>
#include <sys/types.h>

// DWARF libs
#include <dwarf.h>
#include <libdwarf.h>

// modules
#include "reader.h"
#include "parser.h"
#include "executor.h"

// debugger state
bool terminate;
pid_t child_pid;
int bp_counter = 0;
breakpoint * bp_list_head;

// Child state
bool is_loaded;
bool is_running;
char * elf_path = "";

// DWARF vars
Dwarf_Debug dwarf_dbg = 0;
Dwarf_Error dwarf_err;
int elf_fd = -1;

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
		// read
		print_prompt();
		char * input = read_input();

		if (strcmp(input, "\n") != 0 &&
			strcmp(input, "") != 0)
		{
			// tokenzie
			int num_args;
			char ** tokens = tokenize(input, &num_args);

			// parse and execute
			parse(tokens, num_args);
		}

		free(input);
	}

	if (is_loaded)
		do_unload_elf();
	
	return 0;
}

void ignore_int(int throwaway) { }

void print_prompt(void)
{
	printf("(mygdb) ");
}
