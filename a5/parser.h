#ifndef PARSER
#define PARSER

// globals from main that maintain the debugger state
extern bool is_loaded;
extern bool is_running;
extern bool terminate;
extern pid_t child_pid;

// File
const char const * CMD_FILE_LONG = "file";

// Run
const char const * CMD_RUN_LONG = "run";
const char const * CMD_RUN_SHORT = "r";

// Break
const char const * CMD_BREAK_LONG = "break";
const char const * CMD_BREAK_SHORT = "b";

// Continue
const char const * CMD_CONTINUE_LONG = "continue";
const char const * CMD_CONTINUE_SHORT = "c";

// Print
const char const * CMD_PRINT_LONG = "print";
const char const * CMD_PRINT_SHORT = "p";

// Quit
const char const * CMD_QUIT_LONG = "quit";
const char const * CMD_QUIT_SHORT = "q";

char ** tokenize(char * raw_input);
void execute(char ** toekns);

#endif // PARSER
