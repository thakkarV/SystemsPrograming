#include "datastructures.h"

#include <stdlib.h>
#include <sys/ptrace.h>

breakpoint * alloc_breakpoint(void)
{
	breakpoint * bp = calloc(1, sizeof(breakpoint));
	return bp;
}

breakpoint * get_breakpoint(breakpoint * head, unsigned long pc_addr)
{
	while (head != NULL)
	{
		if (head-> bp_addr == pc_addr)
			return head;

		head = head-> next;
	}

	return NULL;
}

void enable_breakpoint(breakpoint * bp)
{
	// first get the bp_data at the bp location
	unsigned long instruction = ptrace(PTRACE_PEEKDATA, child_pid, bp->bp_addr, 0);

	// save it
	bp-> bp_data = (uint8_t) (instruction & 0xFF);
	
	// write int 3 at bp bp_addr
	unsigned long bp_instruction = ((instruction & ~0xFF) | 0xCC);
	ptrace(PTRACE_POKEDATA, child_pid, bp->bp_addr, bp_instruction);
	bp-> is_enabled = true;
}

void disable_breakpoint(breakpoint * bp)
{
	// get value and replace int 3 with original bp_data
	unsigned long instruction = ptrace(PTRACE_PEEKDATA, child_pid, bp->bp_addr, 0);
	unsigned long restored =  ((instruction & ~0xff) | bp-> bp_data);
	ptrace(PTRACE_POKEDATA, child_pid, bp-> bp_addr, restored);
}
