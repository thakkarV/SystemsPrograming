#include "datastructures.h"

// initializes all default fields for the process struct
void init_proc(process * p)
{
	p->argv = malloc(num_args * sizeof(char *));
	p->next = NULL;
	p->pid = -1;
	p->is_completed = false;
	p->is_background = false;
	p->status = -1;
	p->f_stdin = NULL;
	p->f_stdout = NULL;
	p->f_stderr = NULL;
	p->is_piped_next = false;
}

// deallocates all the resources of a process struct
void dealloc_exec_list(process ** exec_list)
{
	process * p;
	int proc_counter = 0;
	while ((p = exec_list[proc_counter++]) != NULL)
	{
		char * arg;
		int arg_counter = 0;
		while (arg = (p->argv[arg_counter++]))
		{
			free(arg);
		}
		free(p->argv);
		free(p->f_stdin);
		free(p->f_stdout);
		free(p->f_stderr);
		free(p);
	}
}
