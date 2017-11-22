#include "printer.h"

void print_matrix(const matrix * in_mat)
{
	for (int row_counter = 0; row_counter < in_mat-> num_rows; row_counter++)
	{
		for (int col_counter = 0; col_counter < in_mat-> num_cols; col_counter++)
		{
			printf("%3d ", in_mat-> elements[row_counter][col_counter]);
		}
		printf("\n");
	}
}

matrix * transpose(const matrix * in_mat)
{
	matrix * new_mat = malloc(sizeof(matrix));
	new_mat-> num_cols = in_mat-> num_rows;
	new_mat-> num_rows = in_mat-> num_cols;

	for (int row_counter = 0; row_counter < in_mat-> num_rows; row_counter++)
	{
		for (int col_counter = 0; col_counter < in_mat-> num_cols; col_counter++)
		{
			 new_mat-> elements[col_counter][row_counter] = in_mat-> elements[row_counter][col_counter];
		}
	}

	return new_mat;
}
