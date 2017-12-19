#include "executor.h"
#include "datastructures.h"

// c utils
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// sys utils
#include <signal.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/ptrace.h>

// dwarf
#include <dwarf.h>
#include <libdwarf.h>


void do_run(const char * path, char ** argv)
{
	if ((child_pid = fork()) == 0)
	{
		// ptrace traceme
		if (ptrace(PTRACE_TRACEME, NULL, NULL, NULL) < 0)
		{
			perror("ptrace");
			exit(1);
		}

		execvp(path, argv);
		perror("execvp");
		exit(1);
	}
	else if (child_pid > 0)
	{
		int ret_val;
		waitpid(child_pid, &ret_val, 0);

		// now enable all breakpoints
		breakpoint * bp = bp_list_head;
		while (bp != NULL)
		{
			if (!(bp-> is_enabled))
				enable_breakpoint(bp);

			bp = bp-> next;
		}

		do_continue();
	}
	else
	{
		perror("fork");
		exit(1);
	}
}

void do_set_breakpoint(unsigned long addr)
{
	breakpoint * bp = alloc_breakpoint();
	bp-> bp_addr = addr;

	if (is_running)
	{
		enable_breakpoint(bp);
		bp-> is_enabled = true;
	}

	bp_list_head-> previous = bp;
	bp-> next = bp_list_head;
	bp-> previous = NULL;
	bp_list_head = bp;
}

void do_unset_breakpoint(unsigned long addr)
{
	// get
	breakpoint * bp = get_breakpoint(bp_list_head, addr);
	// deactivate
	if (bp-> is_enabled)
		disable_breakpoint(bp);
	// splice out
	bp-> previous-> next = bp-> next;
	bp-> next-> previous = bp-> previous;
	if (bp == bp_list_head)
		bp_list_head = bp-> next;
	// dealloc
	free(bp);
}

void do_continue(void)
{
	// resume child
	ptrace(PTRACE_CONT, child_pid, NULL, NULL);

	// now wait on the child
	int status;
	waitpid(child_pid, &status, 0);

	// process status
	process_status(status);
}

void do_print(char * var_name)
{

}

void do_quit(void)
{
	// kill child then terminate
	kill(child_pid, SIGKILL);
	terminate = true;
}

// load ELF. Called by "file" command
void do_load_elf(char * path)
{
	// save path
	elf_path = calloc(1, sizeof(char) * (strlen(path) + 1));
	strcpy(elf_path, path);

	// open elf
	if((fd = open(elf_path, O_RDONLY)) < 0)
	{
		perror("open");
		exit(1);
	}

	// init DWARF libs
	if(dwarf_init(elf_fd, DW_DLC_READ, 0, 0, &dwarf_dbg, &dwarf_err) != DW_DLV_OK)
	{
		fprintf(stderr, "Failed DWARF initialization\n");
		exit(1);
	}
}


// unload ELF. Called by loading a new file in.
void do_unload_elf(void)
{
	// free up the path
	free(elf_path);

	// close the open elf
	close(elf_fd);
	elf_fd = -1;

	// finalize DWARF lib
	if(dwarf_finish(dwarf_dbg, &dwarf_err) != DW_DLV_OK) {
		fprintf(stderr, "Failed DWARF finalization\n");
		return 1;
	}
	dwarf_dbg = 0;
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
unsigned long get_register(unsigned reg_num)
{
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);

	switch (reg_num)
	{
		case rip:
		{
			return regs.rip;
		}
		default:
		{
			return 0;
		}
	}
}


void set_register(unsigned reg_num, unsigned long value)
{
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
	switch (reg_num)
	{
		case rip:
		{
			regs.rip = value;
		}
		default:
		{

		}
	}

	ptrace(PTRACE_SETREGS, child_pid, NULL, &regs);
}

bool getYN(char * prompt)
{
	char line[128];
	do
	{
		printf("%s (y or n) ");
		fgets(line, 128, stdin);

		if (line[0] == 'y')
			return true;
		if (line[0] == 'n')
			return false;

		printf("Please answer y or n.\n");
	} while (true);
}
