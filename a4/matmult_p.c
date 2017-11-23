#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "datastructures.h"
#include "reader.h"
#include "printer.h"
#include "shmops.h"

int main(int argc, char const * argv [])
{

	int shmid;
	void * shmaddr;
	matrix * lhs = get_matrix();
	matrix * rhs = get_matrix();

	if (lhs-> num_cols != rhs-> num_rows)
	{
		printf("Number of columns of the left matrix must match the number of rows of the right matrix.\n");
		return EXIT_FAILURE;
	}

	printf("LHS Matrix :\n");
	print_matrix(lhs);
	printf("\n\n");
	printf("RHS Matrix :\n");
	print_matrix(rhs);


	int num_procs = lhs-> num_rows * rhs-> num_cols;

	int lhs_size =  lhs-> num_rows * lhs-> num_cols + 2;
	int rhs_size = rhs-> num_rows * rhs-> num_cols + 2;
	int new_size = lhs-> num_cols * rhs-> num_rows + 2;
	int shm_size = lhs_size + rhs_size + new_size;

	if ((shmid = shmget(IPC_PRIVATE, shm_size,  IPC_CREAT)) < 0)
	{
		perror("shmget");
		exit(1);
	}

	if ((shmaddr = shmat(shmid, NULL, NULL)) < 0)
	{
		perror("shmat");
		exit(1);
	}

	shmop_serialise_mat(shmaddr, lhs, 0);
	shmop_serialise_mat(shmaddr, rhs, lhs_size)

	return EXIT_SUCCESS;
}
