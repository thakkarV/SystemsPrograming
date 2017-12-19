#include "datastructures.h"

#include <stdlib.h>

breakpoint * alloc_breakpoint(void)
{
	breakpoint * bp = calloc(1, sizeof(breakpoint));
	return bp;
}
