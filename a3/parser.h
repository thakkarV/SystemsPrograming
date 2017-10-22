#ifndef PARSER
#define PARSER

#include <stdbool.h>
#include <stdio.h>

#include "datastructures.h"

const char const * delims = "<|>; \t";
size_t num_procs = 4;
size_t num_args = 4;
process ** parse(char * readbuf);

#endif // PARSER
