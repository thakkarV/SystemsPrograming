#include "shmops.h"

void shmop_serialise_mat(void * shmaddr, const matrix * mat, int offset)
{
	int * current = (int *) shmaddr;

	*(current++) = mat-> num_rows;
	*(current++) = mat-> num_cols;

	for (int i = 0; i < mat-> num_rows; i++)
	{
		for (int j = 0; j < mat-> num_cols; j++)
		{
			*(current++) = mat-> elements[i][j];
		}
	}
}

matrix * shmop_deserialise_mat(void * shmaddr, int offset)
{
	int * current = (int *) shmaddr;

	matrix * mat = malloc(sizeof(matrix));
	mat-> num_rows = *(current++);
	mat-> num_cols = *(current++);

	mat-> elements = malloc(mat-> num_rows * sizeof(int *));
	for (int i = 0; i < mat-> num_rows; i++)
	{
		elements[i] = malloc(mat-> num_cols * sizeof(int))
	}

	for (int i = 0; i < mat-> num_rows; i++)
	{
		for (int j = 0; j < mat-> num_cols; j++)
		{
			 mat-> elements[i][j] = *(current++);
		}
	}

	return mat;
}
