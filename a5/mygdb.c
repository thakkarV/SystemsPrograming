#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

bool is_loaded;
bool is_running;
bool terminate;
pid_t child_pid;
breakpoint * breakpoint

void print_prompt(void);

int main(int argc, char const * argv [])
{
	// register the signal handler first for interrupt
	struct sigaction default_action;
	struct sigaction int_action;
	memset(&int_action, 0, sizeof(sigaction));
	int_action.sa_handler = print_prompt;
	if (sigaction(SIGINT, &int_action, &default_action) < 0)
		perror("sigaction");

	// now start debugger
	is_loaded = false;
	is_running = false;
	terminate  = false;
	while (!terminate)
	{
		char * input = read_input();
		char ** tokens = tokenize(input);
		execute(tokens);
		free(input);
	}

	return 0;
}

void print_prompt(void)
{
	printf("(mygdb) ");
}
