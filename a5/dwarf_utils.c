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

void * get_dwarf_line_addr_from_line(int input_line_num)
{

	Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header;
	Dwarf_Half version_stamp, address_size;
	Dwarf_Error err;
	Dwarf_Die no_die = 0, cu_die, child_die;
	/* Find compilation unit header */
	if(dwarf_next_cu_header(dbg,
							&cu_header_length,
							&version_stamp,
							&abbrev_offset,
							&address_size,
							&next_cu_header,
							&err) == DW_DLV_ERROR)
	{
		die("Error reading DWARF cu header\n");
	}

	/* Expect the CU to have a single sibling - a DIE */
	if(dwarf_siblingof(dbg, no_die, &cu_die, &err) == DW_DLV_ERROR)
	{
		die("Error getting sibling of CU\n");
	}

	int i;
	Dwarf_Error error;
	Dwarf_Signed cnt;
	Dwarf_Line *linebuf;
	bool found = false;
	if(dwarf_srclines(cu_die, &linebuf, &cnt, &error) == DW_DLV_OK)
	{
		for(i = 0; i < cnt; ++i)
		{
			Dwarf_Unsigned line_num;
			Dwarf_Addr line_addr;

			if(dwarf_lineno(linebuf[i], &line_num, &error) != DW_DLV_OK)
			{
				die("Error dwarf_lineno");
			}

			if(dwarf_lineaddr(linebuf[i], &line_addr, &error) != DW_DLV_OK)
			{
				die("Error dwarf_lineno");
			}

			printf("line: %u, addr = %p\n", (unsigned int) line_num, (void *) line_addr);
			if (input_line_num == (unsigned int) line_num)
			{
				bool found = true;
				dwarf_dealloc(dbg, linebuf[i], DW_DLA_LINE);
				dwarf_dealloc(dbg, linebuf, DW_DLA_LIST);
				break;
			}

			dwarf_dealloc(dbg, linebuf[i], DW_DLA_LINE);
		}
		dwarf_dealloc(dbg, linebuf, DW_DLA_LIST);

		if (found)
		{
			return (void *) line_addr;
		}
		else
		{
			printf("No line %d in the current file.\n", input_line_num);
			return NULL;
		}
	}
}

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
		exit(1);
	}
	dwarf_dbg = 0;

	// reset breakpoints
	bp_counter = 0;
	breakpoint * bp = bp_list_head;
	while (bp)
	{
		breakpoint * nbp = bp-> next;
		free(bp);
		bp = nbp
	}
	bp_list_head = NULL;
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
