#ifndef PARSER
#define PARSER

#include "datastructures.h"

#include <stdlib.h>
#include <stdbool.h>

process ** parse(char * readbuf);
void init_proc(process * p);

#endif // PARSER
