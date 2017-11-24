#include "printer.h"

void print_matrix(const matrix * in_mat)
{
	int row_counter;
	int col_counter;
	for (row_counter = 0; row_counter < in_mat-> num_rows; row_counter++)
	{
		for (col_counter = 0; col_counter < in_mat-> num_cols; col_counter++)
		{
			printf("%d", in_mat-> elements[row_counter][col_counter]);
			if (col_counter != in_mat-> num_cols - 1)
				printf(" ");
		}
		printf("\n");
	}
}

matrix * transpose(const matrix * in_mat)
{
	matrix * new_mat = malloc(sizeof(matrix));
	new_mat-> num_cols = in_mat-> num_rows;
	new_mat-> num_rows = in_mat-> num_cols;

	new_mat-> elements = malloc(in_mat-> num_cols * sizeof(int *));
	int i;
	for (i = 0; i < in_mat-> num_cols; i++)
	{
		new_mat-> elements[i] = malloc(in_mat-> num_rows * sizeof(int));
	}

	int row_counter;
	int col_counter;
	for (row_counter = 0; row_counter < in_mat-> num_rows; row_counter++)
	{
		for (col_counter = 0; col_counter < in_mat-> num_cols; col_counter++)
		{
			 new_mat-> elements[col_counter][row_counter] = in_mat-> elements[row_counter][col_counter];
		}
	}

	return new_mat;
}
