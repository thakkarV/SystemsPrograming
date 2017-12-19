#include "executor.h"

#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>

void do_run(const char * prog_name, char ** argv)
{
	if ((child_pid = fork()) == 0)
	{
		// ptrace traceme
		if (ptrace(PTRACE_TRACME, 0, 0, 0) < 0)
		{
			perror("ptrace");
			exit(1);
		}

		execvp(prog_name, argv);
		perror("execvp");
		exit(1);
	}
	else if (child_pid > 0)
	{
		waitpid(child_pid, &ret_val, 0);
		// add all breakpoints here
	}
	else
	{
		perror("fork");
		exit(1);
	}

}

void do_set_breakpoint(int line_num)
{

}

void do_continue()
{
	ptrace(PTRACE_CONT, child_pid, NULL, NULL);

	// now wait on the child
	int ret_val;
	waitpid(child_pid, &ret_val, 0);
}

void do_print()
{

}

void do_quit()
{
	// kill child then kill self
	kill(child_pid, SIGKILL);
	is_running = false;
}

void do_load_elf()
{

}
