#ifndef MATRIX
#define MATRIX

#include <stdlib.h>
#include <ucontext.h>
#include <stdbool.h>

typedef struct matrix
{
	int ** elements;
	int num_rows;
	int num_cols;
} matrix;

// Auxilary methods for matrix
matrix * alloc_matrix(int rows, int cols);
void dealloc_matrix(matrix * mat);

#endif
