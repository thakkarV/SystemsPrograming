#include "datastructures.h"

matrix * alloc_matrix(int rows, int cols)
{
	matrix * mat = malloc(sizeof(matrix));
	mat-> elements = malloc(rows * sizeof(int *));
	int i;
	for (i = 0; i < rows; i++)
	{
		mat-> elements[i] = malloc(cols * sizeof(int));
	}
	
	mat-> num_rows = rows;
	mat-> num_cols = cols;
	return mat;
}

void dealloc_matrix(matrix * mat)
{
	int i;
	for (i = 0; i < mat-> num_rows; i++)
	{
		// free(mat-> elements[i]);
	}
	// free(mat-> elements);
	free(mat);
}
