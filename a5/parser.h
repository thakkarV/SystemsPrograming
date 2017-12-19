#ifndef PARSER
#define PARSER

#include <stdbool.h>
#include <sys/types.h>

// globals from main that maintain the debugger state
extern bool is_loaded;
extern bool is_running;
extern bool terminate;
extern pid_t child_pid;

char ** tokenize(char * raw_input, int * num_tokens);
void execute(char ** toekns, int num_args);

#endif // PARSER
