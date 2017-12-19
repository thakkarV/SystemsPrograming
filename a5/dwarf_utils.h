#ifndef DWARF_UTILS
#define DWARF_UTILS 

#include "datastructures.h"

#include <stdbool.h>
#include <sys/types.h>
#include <dwarf.h>
#include <libdwarf.h>

// globals from main that maintain the debugger state
extern bool is_loaded;
extern bool is_running;
extern bool terminate;
extern char * elf_path;
extern pid_t child_pid;
extern int bp_counter;
extern breakpoint * bp_list_head;

extern Dwarf_Debug dwarf_dbg;
extern Dwarf_Error dwarf_err;
extern int elf_fd;

// load/unload
void do_load_elf(char * path);
void do_unload_elf(void);

// resgister methods
unsigned long get_register(unsigned reg_num);
void set_register(unsigned reg_num, unsigned long value);

// getters
void * get_dwarf_line_addr_from_line(int input_line_num);

// helpers
void die(char* fmt, ...);

#endif
