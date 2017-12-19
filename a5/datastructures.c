#include "datastructures.h"

#include <stdlib.h>
#include <sys/ptrace.h>

// Allocator
breakpoint * alloc_breakpoint(int count)
{
	breakpoint * bp = calloc(1, sizeof(breakpoint));
	bp-> bp_count = count;
	return bp;
}


// Getters
breakpoint * get_breakpoint_by_addr(breakpoint * head, void * addr)
{
	while (head != NULL)
	{
		if (head-> bp_addr == addr)
			return head;

		head = head-> next;
	}

	return NULL;
}

breakpoint * get_breakpoint_by_line(breakpoint * head, unsigned int line_num)
{
	while (head != NULL)
	{
		if (head-> srcfile_line_num == line_num)
			return head;

		head = head-> next;
	}

	return NULL;
}


// Switches
void enable_breakpoint(breakpoint * bp)
{
	// make sure we do not double enable the same breakpoint
	if (bp-> is_enabled)
		return;

	// first get the bp_data at the bp location
	uint64_t  instruction = ptrace(PTRACE_PEEKDATA, child_pid, bp->bp_addr, 0);

	// save it
	bp-> bp_data = (uint8_t) (instruction & 0xFF);
	
	// write int 3 at bp bp_addr
	uint64_t  bp_instruction = ((instruction & ~0xFF) | 0xCC);
	ptrace(PTRACE_POKEDATA, child_pid, bp->bp_addr, bp_instruction);
	bp-> is_enabled = true;
}

void disable_breakpoint(breakpoint * bp)
{
	// get value and replace int 3 with original bp_data
	uint64_t instruction = ptrace(PTRACE_PEEKDATA, child_pid, bp->bp_addr, 0);
	uint64_t restored =  ((instruction & ~0xff) | bp-> bp_data);
	ptrace(PTRACE_POKEDATA, child_pid, bp-> bp_addr, restored);
	bp-> is_enabled = false;
}
