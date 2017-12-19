#include "dwarf_utils.h"
#include "datastructures.h"

// c utils
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


// sys utils
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/ptrace.h>

// dwarf
#include <dwarf.h>
#include <libdwarf.h>

// load ELF. Called by "file" command
void do_load_elf(char * path)
{
	// save path
	elf_path = calloc(1, sizeof(char) * (strlen(path) + 1));
	strcpy(elf_path, path);

	// open elf
	if((elf_fd = open(elf_path, O_RDONLY)) < 0)
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
