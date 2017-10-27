#ifndef PARSER
#define PARSER

#include "datastructures.h"

#include <stdlib.h>
#include <stdbool.h>

process ** parse(char * readbuf);
void init_proc(process * p);
void dealloc_exec_list(process ** exec_list);

#endif // PARSER
