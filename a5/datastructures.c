#include "datastructures.h"

#include <stdlib.h>

breakpoint * alloc_breakpoint(void)
{
	breakpoint * bp = calloc(1, sizeof(breakpoint));
	return bp;
}

breakponint * get_breakpoint(breakpoint * head, uint64_t pc_addr)
{
	while (head != NULL)
	{
		if (head-> instruction_addr == pc_addr)
			return head;

		head = head-> next;
	}

	return NULL;
}
