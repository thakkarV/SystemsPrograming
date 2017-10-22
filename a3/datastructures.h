#ifndef STRUCTS
#define STRUCTS

// implements a linked list of processes
typedef struct process
{
	char ** argv;
	struct process * next; // the next process after this one
	pid_t pid;             // PID of this process
	bool completed;        // true if completed execution, flase if not
	bool is_background;    // true if put into background, false if not
	int status;            // return status from this process, once it has exited
	char * f_stdin;        // filename for redirection of standard in
	char * f_stdout;       // filename for redirection of standard out
	char * f_stderr;       // filename for redirection of standard error
	bool pipe_next;        // true if standard in of next is standard out of this
} process;


// TODO: double linked list of input commands from the user for enabling command history using up/down arrows
typedef struct input
{
	char * input_line;
	struct input * previous;
	struct input * next;
} input;

// initializes all default fields for the process struct
void init_proc(process * p)
{
	p->argv = NULL;
	p->next = NULL;
	p->completed = false;
	p->is_background = false;
	p->pipe_next = false;
}

#endif // STRUCTS
