#include "parser.h"
#include "executor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File
const char const * CMD_FILE_LONG = "file";

// Run
const char const * CMD_RUN_LONG = "run";
const char const * CMD_RUN_SHORT = "r";

// Break
const char const * CMD_BREAK_LONG = "break";
const char const * CMD_BREAK_SHORT = "b";

// Continue
const char const * CMD_CONTINUE_LONG = "continue";
const char const * CMD_CONTINUE_SHORT = "c";

// Print
const char const * CMD_PRINT_LONG = "print";
const char const * CMD_PRINT_SHORT = "p";

// Quit
const char const * CMD_QUIT_LONG = "quit";
const char const * CMD_QUIT_SHORT = "q";

void execute(char ** tokens, int num_args)
{
	if ((strcmp(tokens[0], CMD_QUIT_LONG) == 0) ||
		(strcmp(tokens[0], CMD_QUIT_SHORT) == 0))
	{
		if (is_running)
			do_quit();
		else
			exit(0);
	}
	else if (strcmp(tokens[0], CMD_FILE_LONG) == 0)
	{
		if (!is_loaded)
		{
			do_load_elf(tokens[1]);
			is_loaded = true;
		}
	}
	else if ((strcmp(tokens[0], CMD_BREAK_LONG) == 0) ||
			 (strcmp(tokens[0], CMD_BREAK_SHORT) == 0))
	{
		if (is_loaded)
		{
			do_set_breakpoint(NULL);
		}
	}
	else if ((strcmp(tokens[0], CMD_RUN_LONG) == 0) ||
			 (strcmp(tokens[0], CMD_RUN_SHORT) == 0))
	{
		printf("Starting program: %s\n", elf_path);
		if (!is_loaded)
		{
			printf("No executable file specified.\n");
			printf("Use the \"file\" command.\n");
		}
		else if (is_loaded && is_running)
		{

		}
		else if (is_loaded && !is_running)
		{
			is_running = true;
			do_run(elf_path, &tokens[1]);
		}
	}
	else if ((strcmp(tokens[0], CMD_CONTINUE_LONG) == 0) ||
			 (strcmp(tokens[0], CMD_CONTINUE_SHORT) == 0))
	{
		if (is_running)
			do_continue();
		else
			printf("No program currenrly running.\n");
	}
	else if ((strcmp(tokens[0], CMD_PRINT_LONG) == 0) ||
			 (strcmp(tokens[0], CMD_PRINT_SHORT) == 0))
	{
		if (is_running)
			do_print();	
		else
			printf("No program currenrly running.\n");
	}
	else
	{
		printf("Undefined command: \"%s\".\n", tokens[0]);
	}
}


// tokenizes a string command simply by spaces
char ** tokenize(char * command, int * num_tokens)
{
	int DEFAULT_NUM_TOKS = 2;
	char ** tokens = malloc(DEFAULT_NUM_TOKS * sizeof(char *));
	char * token = strtok(command, " ");
	
	*num_tokens = 0;
	while (token != NULL)
	{	
		if (strlen(token) > 0)
			tokens[(*num_tokens)++] = token;

		token = strtok(NULL, " ");

		// realloc if necessary
		if ((*num_tokens) >= DEFAULT_NUM_TOKS)
		{
			DEFAULT_NUM_TOKS *= 2;
			tokens = realloc(tokens, DEFAULT_NUM_TOKS * sizeof(char *));

			if (!tokens)
			{
				perror("realloc");
			}
		}
	}

	return tokens;
}


