#ifndef EXECUTOR
#define EXECUTOR

#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

// globals from main that maintain the debugger state
extern bool is_running;
extern bool terminate;
extern pid_t child_pid;


#include "executor.h"

void do_run();

void do_set_breakpoint(int line_num);

void do_continue();

void do_print();

void do_quit();

void do_load_elf();


#endif // EXECUTOR
