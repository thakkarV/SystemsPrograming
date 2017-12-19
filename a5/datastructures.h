#ifndef DATASTRCUTURES
#define DATASTRCUTURES

#include <stdlib.h>
#include <stdbool.h>

typedef struct breakpoint
{
	// line number in the source file where the breakpoint is to be set
	int srcfile_line_num;

	// first instruction representing the line number to be replaced by 0xCC
	char instruction;

	// address in the text of the running program where the insturction is
	void * instruction_addr;

	// true if the breakpoint is active in the source
	bool is_active;
	
} breakpoint;

breakpoint * alloc_breakpoint(void);

#endif
