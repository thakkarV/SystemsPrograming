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

	breakpoint * next;
	
} breakpoint;

// allocator
breakpoint * alloc_breakpoint(void);

// helper methods for the breakpoint struct
breakponint * get_breakpoint(uint64_t addr);


enum REGS
{
    rax, rbx, rcx, rdx,
    rdi, rsi, rbp, rsp,
    r8,  r9,  r10, r11,
    r12, r13, r14, r15,
    rip, rflags,    cs,
    orig_rax, fs_base,
    gs_base,
    fs, gs, ss, ds, es
};

#endif
