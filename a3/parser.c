#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructures.h"

const char const * delims = "<|>;& \t";
size_t num_procs = 4;
size_t num_args = 4;

// converts the commands seperated by tokenizer into processes to be executed
process ** parse(char * readbuf)
{
	process ** procs = malloc(num_procs * sizeof(process *));
	process * p = malloc(sizeof(process));
	process * p_next = NULL;
	int proc_index = 0;
	int proc_args_index = 0;
	char ** args = malloc(num_args * sizeof(char *));

	char * start_ptr = readbuf;
	char * end_ptr = strpbrk(start_ptr, delims);
	char * delim;
	char * previous_delim;

	init_proc(p);
	while (end_ptr != NULL)
	{
		delim = end_ptr;
		switch (*delim)
		{
			case ';': // new cmd
			{
				procs[proc_index++] = p;
				p_next = malloc(sizeof(process));
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
			case '<': // stdin redir
			{
				char * filename = malloc(end_ptr - start_ptr);
				strncpy(filename, start_ptr, end_ptr - start_ptr);
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
				p->pipe_next = true;
				procs[proc_index++] = p;
				p_next = malloc(sizeof(process));
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
			case '\t': // fall through to space
			case ' ':  // space
			{
				if (strpbrk(delims, previous_delim) == NULL)
				{
					proc_args_index++;
					p->argv = realloc(p->argv, proc_args_index * sizeof(char *));
					if (!(p->argv))
					{
						perror("realloc");
					}

					char * arg = malloc(end_ptr - start_ptr);
					strncpy(arg, start_ptr, end_ptr	- start_ptr);
					p->argv[proc_args_index - 1] = arg;
				}
				break;
			}
		}

		start_ptr = end_ptr;
		end_ptr = strpbrk(start_ptr, delims);
		*previous_delim = *delim;
	}

	// the last process might have been missing if no ';' was at the end
	if (procs[proc_index - 1] != p)
		procs[proc_index] = p;

	return procs;
}

// initializes all default fields for the process struct
void init_proc(process * p)
{
	p->argv = NULL;
	p->next = NULL;
	p->completed = false;
	p->is_background = false;
	p->f_stdin = NULL;
	p->f_stdout = NULL;
	p->f_stderr = NULL;
	p->pipe_next = false;
}
