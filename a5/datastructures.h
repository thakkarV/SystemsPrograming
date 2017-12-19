#ifndef DATASTRCUTURES
#define DATASTRCUTURES

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

extern bool is_loaded;
extern bool is_running;
extern bool terminate;
extern char * elf_path;
extern pid_t child_pid;

typedef struct breakpoint
{
	// counter for this break point
	int bp_count;

	// line number in the source file where the breakpoint is to be set
	int srcfile_line_num;

	// first instruction representing the line number to be replaced by 0xCC
	uint8_t bp_data;

	// address in the text of the running program where the insturction is
	void * bp_addr;

	// true if the breakpoint is active in the source
	bool is_enabled;

	// doubly linked list
	struct breakpoint * previous;
	struct breakpoint * next;
	
} breakpoint;

// allocator
breakpoint * alloc_breakpoint(int count);

// helper methods for the breakpoint struct
breakpoint * get_breakpoint_by_addr(breakpoint * head, void * addr);
breakpoint * get_breakpoint_by_line(breakpoint * head, unsigned int line_num);
void enable_breakpoint(breakpoint * bp);
void disable_breakpoint(breakpoint * bp);

enum REGISTERS
{
    rax, rbx, rcx, rdx,
    rdi, rsi, rbp, rsp,
    r8,  r9,  r10, r11,
    r12, r13, r14, r15,
    rip, rflags, cs,
    orig_rax, fs_base, gs_base,
    fs, gs, ss, ds, es
};

#endif
