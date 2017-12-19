#ifndef PARSER
#define PARSER

#include <stdbool.h>
#include <sys/types.h>

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

char ** tokenize(char * raw_input, int * num_tokens);
void parse(char ** toekns, int num_args);

#endif // PARSER
