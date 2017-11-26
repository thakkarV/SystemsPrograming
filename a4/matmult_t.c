#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "datastructures.h"
#include "reader.h"
#include "printer.h"
#include "shmops.h"
#include "threads.h"

void multiply(int tid);

// golbals for argument juggling
matrix * lhs;
matrix * rhs;
matrix * new_mat;

int main(int argc, char const * argv [])
{
	lhs = get_matrix();
	rhs = get_matrix();

	if (lhs-> num_cols != rhs-> num_rows)
	{
		printf("Number of columns of the left matrix must match the number of rows of the right matrix.\n");
		return EXIT_FAILURE;
	}

	int num_threads = lhs-> num_rows * rhs-> num_cols;
	int lhs_size = lhs-> num_rows * lhs-> num_cols + 2;
	int rhs_size = rhs-> num_rows * rhs-> num_cols + 2;
	int new_size = lhs-> num_cols * rhs-> num_rows + 2;

	// alloc the output vector at global scope

	new_mat = alloc_matrix(lhs-> num_rows, rhs-> num_cols);
	int i;
	for (i = 0; i < num_threads; i++)
	{
		my_thr_create(multiply, i);
	}

	start_threads();

	print_matrix(new_mat);

	dealloc_matrix(lhs);
	dealloc_matrix(rhs);
	dealloc_matrix(new_mat);

	return 0;
}

void multiply(int tid)
{
	int output = 0;
	int lhs_row = (int) tid / lhs-> num_rows;
	int rhs_col = (int) tid % rhs-> num_cols;
	int i;
	for (i = 0; i < lhs-> num_cols; i++)
	{
		output += lhs-> elements[lhs_row][i] * rhs-> elements[i][rhs_col];
	}

	new_mat-> elements[lhs_row][rhs_col] = output;
}
