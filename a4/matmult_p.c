#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "datastructures.h"
#include "reader.h"
#include "printer.h"
#include "shmops.h"

int main(int argc, char const * argv [])
{
	// read input from stdin
	int shmid;
	void * shmaddr;
	matrix * lhs = get_matrix();
	matrix * rhs = get_matrix();

	if (lhs-> num_cols != rhs-> num_rows)
	{
		printf("Number of columns of the left matrix must match the number of rows of the right matrix.\n");
		return EXIT_FAILURE;
	}

	// setup and get shared memory
	int num_procs = lhs-> num_rows * rhs-> num_cols;
	int lhs_size = lhs-> num_rows * lhs-> num_cols + 2;
	int rhs_size = rhs-> num_rows * rhs-> num_cols + 2;
	int new_size = lhs-> num_cols * rhs-> num_rows + 2;
	int shm_size = 4 * (lhs_size + rhs_size + new_size);

	if ((shmid = shmget(IPC_PRIVATE, shm_size, S_IRWXU | S_IRWXG | IPC_CREAT)) < 0)
	{
		perror("shmget");
		exit(1);
	}

	if ((shmaddr = shmat(shmid, NULL, 0)) < 0)
	{
		perror("shmat");
		exit(1);
	}

	// serialize
	shmop_serialize_mat(shmaddr, lhs, 0);
	shmop_serialize_mat(shmaddr, rhs, lhs_size);
	int * setupaddr = (int *) shmaddr;
	setupaddr += lhs_size + rhs_size;
	*(setupaddr++) = lhs-> num_rows;
	*(setupaddr++) = rhs-> num_cols;

	// execute
	int i;
	int retval;
	pid_t * child_pids = malloc(num_procs * sizeof(pid_t));
	for (i = 0; i < num_procs; i++)
	{
		
		if ((retval = fork()) == -1)
		{
			perror("fork");
			exit(1);
		}

		if (retval == 0)
		{
			char s_counter[16];
			sprintf(s_counter, "%d", i);

			char s_shmid[16];
			sprintf(s_shmid, "%d", shmid);

			char s_lhs_rows[16];
			sprintf(s_lhs_rows, "%d", lhs-> num_rows);

			char s_inner_dim[16];
			sprintf(s_inner_dim, "%d", lhs-> num_cols);

			char s_rhs_cols[16];
			sprintf(s_rhs_cols, "%d", rhs-> num_cols);

			execl("./multiply", "multiply", s_counter, s_shmid, s_lhs_rows, s_inner_dim, s_rhs_cols, (char *) NULL);

			perror("execl");
			exit(1);
		}
		else
		{
			child_pids[i] = retval;
		}
	}

	// wait for all children
	for (i = 0; i < num_procs; i++)
	{
		waitpid(child_pids[i], &retval, 0);
	}

	// deserialise output matrix
	matrix * new_mat = shmop_deserialize_mat(shmaddr, lhs_size + rhs_size);
	print_matrix(new_mat);

	// clean up and exit
	if (shmctl(shmid, IPC_RMID, NULL) < 0)
	{
		perror("shmctl");
		exit(1);
	}

	if (shmdt(shmaddr) < 0)
	{
		perror("shmdt");
		exit(1);
	}

	return EXIT_SUCCESS;
}
