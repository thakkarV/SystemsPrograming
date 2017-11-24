#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char const * argv[])
{
	if (argc != 6)
	{
		printf("Not enough input args. Got %d.\n", argc);
		exit(1);
	}
	
	// read in cmd args
	int counter = atoi(argv[1]);
	int shmid = atoi(argv[2]);
	int num_lhs_rows = atoi(argv[3]);
	int inner_dim = atoi(argv[4]);
	int num_rhs_col = atoi(argv[5]);

	// attach
	void * shmaddr;
	if ((shmaddr = shmat(shmid, NULL, 0)) < 0)
	{
		perror("shmat");
		exit(1);
	}

	// calc offsets
	int * lhs_loc = (int *) shmaddr + 2;
	int * rhs_loc = (int *) shmaddr + num_lhs_rows * inner_dim + 4;
	int * output_loc = (int *) shmaddr + num_lhs_rows * inner_dim + 2
									   + inner_dim * num_rhs_col + 2
									   + 2 + counter;

	// multiply
	int output = 0;
	int lhs_row = (int) counter / num_lhs_rows;
	int rhs_col = (int) counter % num_rhs_col;

	int i;
	for (i = 0; i < inner_dim; i++)
	{
		output += (*(lhs_loc + lhs_row * inner_dim + i)) * (*(rhs_loc + num_rhs_col * i + rhs_col));
	}

	// output
	*output_loc = output;

	// detach
	if (shmdt(shmaddr) < 0)
	{
		perror("shmdt");
		exit(1);
	}

	return 0;
}
