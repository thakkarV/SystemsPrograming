#include "shmops.h"

void shmop_serialize_mat(const void * shmaddr, const matrix * mat, int offset)
{
	int i, j;
	int * current = (int *) shmaddr + offset;

	*(current++) = mat-> num_rows;
	*(current++) = mat-> num_cols;

	for (i = 0; i < mat-> num_rows; i++)
	{
		for (j = 0; j < mat-> num_cols; j++)
		{
			*(current++) = mat-> elements[i][j];
		}
	}
}

matrix * shmop_deserialize_mat(const void * shmaddr, int offset)
{
	int i, j;
	int * current = (int *) shmaddr + offset;

	matrix * mat = malloc(sizeof(matrix));
	mat-> num_rows = *(current++);
	mat-> num_cols = *(current++);

	mat-> elements = malloc(mat-> num_rows * sizeof(int *));
	for (i = 0; i < mat-> num_rows; i++)
	{
		mat-> elements[i] = malloc(mat-> num_cols * sizeof(int));
	}

	for (i = 0; i < mat-> num_rows; i++)
	{
		for (j = 0; j < mat-> num_cols; j++)
		{
			 mat-> elements[i][j] = *(current++);
		}
	}

	return mat;
}
