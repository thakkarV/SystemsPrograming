#ifndef PARSER
#define PARSER

const char const * delims = "<|>;& \t";
size_t num_procs = 4;
size_t num_args = 4;

process ** parse(char * readbuf);
void init_proc(process * p);

#endif // PARSER
