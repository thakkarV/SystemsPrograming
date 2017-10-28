#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructures.h"

const char const * delims = "<|>&\t\n";
size_t num_procs = 4;
size_t num_args = 4;

// converts the commands seperated by tokenizer into processes to be executed
process ** parse(char * readbuf)
{
	char ** commands = malloc(default_num_commands * sizeof(char *));
	char * tmp;
	int num_commands = 0;


	// split over all ';' first
	tmp = strtok(readbuf, ";");
	while (tmp != NULL)
	{
		commands[num_commands++] = tmp;
		tmp = strtok(NULL, ";");

		// extend if necessary
		if (num_commands >= default_num_commands)
		{
			default_num_commands += default_num_commands;
			commands = realloc(commands, default_num_commands * sizeof(process *));
			if (!commands)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// init proc list
	process ** procs = calloc(num_procs * sizeof(process *));
	process * p = malloc(sizeof(process));
	init_proc(p);

	int proc_index = 0;
	

	// now parse each individual commands
	for(int cmd = 0; i < num_commands; i++)
	{
		int num_tokens = 0;
		int proc_args_index = 0;

		// first get all space delimited tokens
		char ** tokens = tokenize(char * commands[cmd], &num_tokens);

		char previous_delim = ' ';
		// now parse all of them and add to proc list
		for (int tk = 0; tk < num_tokens; tk++)
		{
			switch (previous_delim)
			{
				case '\t': // fall through to space
				case '\n':
				case ' ':  // space
				{

					break;
				}
				case '<': // stdin redir
				{
					p->f_stdin = filename;
					break;
				}
				case '>': // stdout redir
				{
					// this is a compicated case, we have to check for 1> or 2>, %> will be caught by the & case
					char previous_char = *(end_ptr - 1);
					switch (previous_char)
					{
						case '&': // will be handled by '&' case
						{
							break;
						}
						case '1': // fall through for 1 or space
						case ' ': // stdout redirection
						{
							char * filename = malloc(end_ptr - start_ptr);
							strncpy(filename, start_ptr, end_ptr - start_ptr);
							p-> f_stdout = filename;
							break;
						}
						case '2': // stderr redirection file
						{
							char * filename = malloc(end_ptr - start_ptr);
							strncpy(filename, start_ptr, end_ptr - start_ptr);
							p-> f_stderr = filename;
							break;
						}
					}
					break;
				}
				case '&': // background
				{
					// 2 possible cases: & by itself for backgorunding or &> for IO redir
					if (*(end_ptr + 1) == '>') // stdout and stderr redir
					{
						char * filename = malloc(end_ptr - start_ptr);
						strncpy(filename, start_ptr, end_ptr - start_ptr);
						p->f_stdout = filename;
						p->f_stderr = filename;
					}
					else // background
					{
						p->is_background = true;
					}
					break;
				}
				case '|':  // pipe
				{
					p->is_piped_next = true;
					procs[proc_index++] = p;
					process * p_next = malloc(sizeof(process));
					init_proc(p_next);
					p->next = p_next;
					p = p_next;

					if (proc_index >= num_procs)
					{
						num_procs += num_procs;
						procs = realloc(procs, num_procs * sizeof(process *));
						if (!procs)
						{
							perror("realloc");
							exit(EXIT_FAILURE);
						}
					}
					break;
				}
			}

			// assign previous_delim here
		}



		// realloc if necessary
		if (num_procs >= default_num_procs)
		{
			default_num_procs *= 2;
			procs = realloc(default_num_procs * sizeof(char *));

			if (!procs)
			{
				perror("realloc");
			}
		}
	}
}


// tokenizes a string command simply by spaces
char ** tokenize(char * command, int * num_tokens)
{
	char ** tokens = malloc(default_num_tokens * sizeof(char *));
	char * token = strtok(command, " ");
	
	*num_tokens = 0;
	while (token != NULL)
	{
		tokens[(*num_tokens)++] = token;
		token = strtok(NULL, " ");

		// realloc if necessary
		if ((*num_tokens) >= default_num_tokens)
		{
			default_num_tokens *= 2;
			tokens = realloc(default_num_tokens * sizeof(char *));

			if (!tokens)
			{
				perror("realloc");
			}
		}
	}
}
