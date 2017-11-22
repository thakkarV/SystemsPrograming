#include <stdio.h>
#include <stdlib.h>

#include "datastructures.h"
#include "reader.h"
#include "printer.h"

int main(int argc, char const * argv [])
{
	matrix * lhs = get_matrix();
	matrix * rhs = get_matrix();

	if (lhs-> num_cols != rhs-> num_rows)
	{
		printf("Number of columns of the left matrix must match the number of rows of the right matrix.\n")
		return EXIT_FAILURE;
	}

	int num_procs = lhs-> num_rows * rhs-> num_cols;

	printf("LHS Matrix :\n");
	print_matrix(lhs);
	printf("\n\n");
	printf("RHS Matrix :\n");
	print_matrix(rhs);

	return EXIT_SUCESS;
}
