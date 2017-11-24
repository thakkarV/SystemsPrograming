#include "reader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "datastructures.h"

#define BUF_SIZE 8192

matrix * get_matrix(void)
{
	matrix * mat;
	int row_counter = 0;
	int col_counter = 0;
	int curr_col_counter = 0;
	int prev_col_counter = 0;
	int curr_max_rows = 4;
	int curr_max_cols = 4;
	int entry = 0;
	int * row;
	char * token;
	char buf[BUF_SIZE];

	mat = malloc(sizeof(matrix));
	mat-> elements = malloc(curr_max_rows * sizeof(int *));

	// first get only on row, count num cols using that
	row = malloc(curr_max_cols * sizeof(int));
	fgets(buf, BUF_SIZE, stdin);
	token = strtok(buf, " ");
	while (token != NULL)
	{
		entry = strtol(token, NULL, 10);
		row[col_counter] = entry;
		col_counter++;
		// expand row if needed
		if (col_counter >= curr_max_cols)
		{
			curr_max_cols *= 2;
			row = realloc(row, curr_max_cols);
			if (!row)
			{
				perror("realloc");
				exit(1);
			}
		}

		token = strtok(NULL, " ");
	}


	mat-> num_cols = col_counter;
	mat-> elements[row_counter++] = row;

	// now get rest of the rows one by one while checking nun_cols
	while(fgets(buf, BUF_SIZE, stdin) != NULL)
	{
		if (buf[0] == '\n')
			break;

		token = strtok(buf, " ");

		// one empty row
		row = malloc(curr_max_cols * sizeof(int));

		col_counter = 0;
		while (token != NULL)
		{
			entry = strtol(token, NULL, 10);
			row[col_counter++] = entry;			
			// expand row if needed
			if (col_counter >= curr_max_cols)
			{
				curr_max_cols *= 2;
				row = realloc(row, curr_max_cols);

				if (!row)
				{
					perror("realloc");
					exit(1);
				}
			}

			token = strtok(NULL, " ");
		}

		if (col_counter != mat-> num_cols)
		{
			fprintf(stderr, "Input matrix has different number of elements per row.\n");
			exit(1);
		}

		// add row to matrix
		mat->elements[row_counter++] = row;

		// increase row count if needed
		if (row_counter >= curr_max_rows)
		{
			curr_max_rows *= 2;
			mat->elements = realloc(mat-> elements, curr_max_rows);
			if (!mat-> elements)
			{
				perror("realloc");
			}
		}
	}

	mat-> num_rows = row_counter;
	return mat;
}
