#ifndef EXECUTOR
#define EXECUTOR

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include "executor.h"
#include "datastructures.h"

// globals from main that maintain the debugger state
extern bool is_loaded;
extern bool is_running;
extern bool terminate;
extern char * elf_path;
extern pid_t child_pid;
extern breakpoint * bp_list_head;

extern Dwarf_Debug dwarf_dbg;
extern Dwarf_Error dwarf_err;
extern int elf_fd;


// commands processors
void do_run();
void do_set_breakpoint(unsigned long addr);
void do_unset_breakpoint(unsigned long addr);
void do_continue();
void do_print();
void do_quit();

// wait dealers
void process_status(int status);

// helpers
bool getYN(char * prompt);

#endif // EXECUTOR
