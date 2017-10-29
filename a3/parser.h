#ifndef PARSER
#define PARSER

#include "datastructures.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ONE_GT 1
#define TWO_GT 2
#define AND_GT 3
#define LT 4
#define PIPE 5
#define SPACE 6
#define AMPERSAND 7

process ** parse(char * readbuf);
char ** tokenize(char * command, int * num_tokens);
int check_delim(char * token);


#endif // PARSER
