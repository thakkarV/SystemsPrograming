#include <stdio.h>

#include "datastructures.h"
#include "reader.h"
#include "printer.h"

int main(int argc, char const * argv [])
{
	matrix * mat = get_matrix();
	matrix * mat_transpose = transpose(mat);
	print_matrix(mat_transpose);
	return 0;
}
