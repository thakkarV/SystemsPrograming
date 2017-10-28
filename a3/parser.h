#ifndef PARSER
#define PARSER

#include "datastructures.h"

#include <stdlib.h>
#include <stdbool.h>

extern int num_args;
int default_num_commands = 2;
int default_num_tokens = 2;

process ** parse(char * readbuf);
char ** tokenize(char * command);


#endif // PARSER
