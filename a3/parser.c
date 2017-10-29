#include "parser.h"

const char const * delims = "<|>&\t\n";

int default_num_commands = 2;
int default_num_tokens = 2;
int default_num_procs = 2;

extern int default_num_args;

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
			default_num_commands *= 2;
			commands = realloc(commands, default_num_commands * sizeof(process *));
			if (!commands)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// init proc list
	process ** procs = malloc(default_num_procs * sizeof(process *));

	int num_procs = 0;
	char ** cpy_commands = commands;
	// now parse each individual commands
	for(int cmd = 0; cmd < num_commands; cmd++)
	{
		process * p = malloc(sizeof(process));
		init_proc(p);

		int num_tokens = 0;
		int proc_args_index = 0;

		// first get all space delimited tokens
		char ** tokens = tokenize(*(cpy_commands++), &num_tokens);

		int num_args = 0;
		int current_delim;
		int previous_delim = check_delim(tokens[0]);
		// now parse all of them and add to proc list
		for (int tk = 0; tk < num_tokens; tk++)
		{
			current_delim = check_delim(tokens[tk]);
			if (current_delim == PIPE)
			{
				// redir file to f_stdin
				p-> is_piped_next = true;
				procs[num_procs] = p;
				p = malloc(sizeof(process));
				init_proc(p);
				procs[num_procs]-> next = p;
				num_procs++;
				current_delim = SPACE;
				continue;
			}
			else if (previous_delim == SPACE)
			{
				if (check_delim(tokens[tk]) == previous_delim)
				{
					// add token to argv
					p-> argv[num_args++] = tokens[tk];

					// realloc if necessary
					if (num_args >= default_num_args)
					{
						default_num_args *= 2;
						p-> argv = realloc(p-> argv, default_num_args * sizeof(char *));
						if (!(p-> argv))
						{
							perror("realloc");
						}
					}	
				}
			}
			else if (previous_delim == ONE_GT)
			{
				// redir f_stdout to file
				p-> f_stdout = tokens[tk];
			}
			else if (previous_delim == TWO_GT)
			{
				// redir to f_stderr only
				p-> f_stderr = tokens[tk];
			}
			else if (previous_delim == AND_GT)
			{
				// redir to f_stderr only
				p-> f_stdout = tokens[tk];
				p-> f_stderr = tokens[tk];
			}
			else if (previous_delim == LT)
			{
				// redir strout and strerr to file
				p-> f_stdin = tokens[tk];
			}
			else if (previous_delim == AMPERSAND)
			{
				// redir file to f_stdin
				p-> is_background = true;
			}

			
			// realloc procs if necessary
			if (num_procs >= default_num_procs)
			{
				default_num_procs *= 2;
				procs = realloc(procs, default_num_procs * sizeof(char *));

				if (!procs)
				{
					perror("realloc");
				}
			}

			previous_delim = current_delim;
		}

		if (previous_delim == AMPERSAND)
			p-> is_background = true;

		if (previous_delim != PIPE)
		{
			procs[num_procs++] = p;
		}

		free(tokens);
		tokens = NULL;
	}

	free(commands);
	return procs;
}


// tokenizes a string command simply by spaces
char ** tokenize(char * command, int * num_tokens)
{
	char ** tokens = malloc(default_num_tokens * sizeof(char *));
	char * token = strtok(command, " ");
	
	*num_tokens = 0;
	while (token != NULL)
	{	
		if (strlen(token) > 0)
			tokens[(*num_tokens)++] = token;

		token = strtok(NULL, " ");

		// realloc if necessary
		if ((*num_tokens) >= default_num_tokens)
		{
			default_num_tokens *= 2;
			tokens = realloc(tokens, default_num_tokens * sizeof(char *));

			if (!tokens)
			{
				perror("realloc");
			}
		}
	}

	return tokens;
}


int check_delim(char * token)
{

	if (strcmp(token, ">") == 0
			|| strcmp(token, "1>") == 0)
	{
		// redir f_stdout to file
		return ONE_GT;
	}
	else if (strcmp(token, "2>") == 0)
	{
		// redir to f_stderr only
		return TWO_GT;
	}
	else if (strcmp(token, "&>") == 0)
	{
		// redir strout and strerr to 
		return AND_GT;
	}
	else if (strcmp(token, "<") == 0)
	{
		// redir file to f_stdin
		return LT;
	}
	else if (strcmp(token, "|") == 0)
	{
		// pipe to next 
		return PIPE;
	}
	else if (strcmp(token, "&") == 0)
	{
		// background
		return AMPERSAND;
	}
	else
	{
		// add token to argv
		return SPACE;
	}				
}