#include "executor.h"

#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>

void do_run(const char * prog_name, char ** argv)
{
	if ((child_pid = fork()) == 0)
	{
		// ptrace traceme
		if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0)
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
		int ret_val;
		waitpid(child_pid, &ret_val, 0);
		// add all breakpoints here
	}
	else
	{
		perror("fork");
		exit(1);
	}

}

void do_set_breakpoint(void * addr)
{

}

void do_continue()
{
	// resume child
	ptrace(PTRACE_CONT, child_pid, NULL, NULL);

	// now wait on the child
	int status;
	waitpid(child_pid, &status, 0);

	// process status
	process_status(status);
}

void do_print()
{

}

void do_quit()
{
	// kill child then terminate
	kill(child_pid, SIGKILL);
	exit(0);
}

void do_load_elf(char * path)
{
	elf_path = calloc(1, sizeof(char) * (strlen(path) + 1));
	strcpy(elf_path, path);
}

void process_status(int status)
{
	if (WIFEXITED(status))
	{
		int ret_val = WEXITSTATUS(status);
		if (ret_val == 0)
			printf("Program exited normally.\n");
		else
			printf("Program exited with code %d.\n", ret_val);

		is_running = 0;
	}
	else if (WIFSIGNALED(status))
	{
		printf("Signal %d sent to child.\n", WTERMSIG(status));
	}
}


// resgister methods
uint64_t get_register(pid_t pid, unsigned reg_num)
{
	
}

void set_register(pid_t pid, unsigned reg_num, uint64_t value)
{

}
