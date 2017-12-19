#include "datastructures.h"

#include <stdlib.h>

breakpoint * alloc_breakpoint(void);
{
	breakpoint * bp = calloc(sizeof(breakpoint));
	return bp;
}
