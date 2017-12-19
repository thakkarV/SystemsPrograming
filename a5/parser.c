#include "parser.h"
#include "executor.h"

#include <string.h>

void executor(char ** tokens)
{
	if (strcmp(tokens[0], CMD_FILE_LONG))
	{
		if (!is_loaded)
		{
			do_load_elf();
			is_loaded = true;
		}
	}
	else if ((strcmp(tokens[0], CMD_BREAK_LONG) == 0) || (strcmp(tokens[0], CMD_BREAK_SHORT) == 0))
	{
		if (is_loaded)
		{
			do_set_breakpoint();
		}
	}
	else if ((strcmp(tokens[0], CMD_RUN_LONG) == 0) || (strcmp(tokens[0], CMD_RUN_SHORT) == 0))
	{
		if (!is_loaded)
		{

		}
		else if (is_loaded && is_running)
		{

		}
		else if (is_loaded && !is_running)
		{
			is_running = true;
			do_run(tokens[1], &tokens[1]);
		}
	}
	else if ((strcmp(tokens[0], CMD_CONTINUE_LONG) == 0) || (strcmp(tokens[0], CMD_CONTINUE_SHORT) == 0))
	{
		if (is_running)
		{
			do_continue();
		}
	}
	else if ((strcmp(tokens[0], CMD_PRINT_lONG) == 0) || (strcmp(tokens[0], CMD_PRINT_SHORT) == 0))
	{
		if (is_running)
		{
			do_print();	
		}
	}
	else if ((strcmp(tokens[0], CMD_QUIT_LONG) == 0) || (strcmp(tokens[0], CMD_QUIT_SHORT) == 0))
	{
		if (is_running)
		{
			do_quit();
		}
	}
	else
	{
		printf("Undefined command: \"%s\".", tokens[0]);
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


