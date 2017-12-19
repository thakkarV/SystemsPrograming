#include "parser.h"
#include "executor.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

// File
const char const * CMD_FILE_LONG = "file";

// Run
const char const * CMD_RUN_LONG = "run";
const char const * CMD_RUN_SHORT = "r";

// Set Break
const char const * CMD_BREAK_LONG = "break";
const char const * CMD_BREAK_SHORT = "b";

// Clear Breakpoint
const char const * CMD_CLEAR_LONG = "clear";

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
	// QUIT
	if ((strcmp(tokens[0], CMD_QUIT_LONG) == 0) ||
		(strcmp(tokens[0], CMD_QUIT_SHORT) == 0))
	{
		if (is_running)
			do_quit();
		else
			exit(0);
	}
	// LOAD ELF
	else if (strcmp(tokens[0], CMD_FILE_LONG) == 0)
	{
		if (!is_running)
		{
			if (is_loaded)
				do_unload_elf();
			
			do_load_elf(tokens[1]);
			is_loaded = true;
		}
		else
		{
			if (getYN("A program is being debugged already.\nAre you sure you want to change the file?"))
			{
				kill(child_pid, SIGKILL);
				do_load_elf(tokens[1]);
			}
			else
			{
				printf("File not changed.\n");
			}
		}
	}
	// SET BREAKPOINT
	else if ((strcmp(tokens[0], CMD_BREAK_LONG) == 0) ||
			 (strcmp(tokens[0], CMD_BREAK_SHORT) == 0))
	{
		if (is_loaded)
		{
			// figure out DWARF shit here
			// first distinguish between number and name
			// if line num, get line num addr
			// if func name, get start of func addr
			do_set_breakpoint(0);
		}
		else
		{
			printf("No symbol table is loaded.  Use the \"file\" command.\n");
		}
	}
	// CLEAR BREAKPOINT
	else if (strcmp(tokens[0], CMD_CLEAR_LONG) == 0)
	{
		if (is_loaded)
		{
			// figure out DWARF shit here
			// first distinguish between number and name
			// if line num, get line num addr
			// if func name, get start of func addr
			do_unset_breakpoint(0);
		}
		else
		{
			printf("No symbol table is loaded.  Use the \"file\" command.\n");
		}
	}
	// START PROGRAM
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
			if (getYN("The program being debugged has been started already.\nStart it from the beginning?"))
			{
				kill(child_pid, SIGKILL);
				do_run(elf_path, &tokens[1]);
			}
			else
			{
				printf("Program not restarted.\n");
			}
		}
		else if (is_loaded && !is_running)
		{
			is_running = true;
			do_run(elf_path, &tokens[1]);
		}
	}
	// CONTINUE EXECUTION
	else if ((strcmp(tokens[0], CMD_CONTINUE_LONG) == 0) ||
			 (strcmp(tokens[0], CMD_CONTINUE_SHORT) == 0))
	{
		if (is_running)
			do_continue();
		else
			printf("No program currenrly running.\n");
	}
	// PRINT VARIABLE
	else if ((strcmp(tokens[0], CMD_PRINT_LONG) == 0) ||
			 (strcmp(tokens[0], CMD_PRINT_SHORT) == 0))
	{
		if (is_running)
			do_print();	
		else
			printf("No program currenrly running.\n");
	}
	// UNRECOGNISED COMMAND
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
