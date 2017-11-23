#ifndef SHMOPS
#define SHMOPS


#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "datastructures.h"

void shmop_serialise_mat(void * shmaddr, const matrix * mat, int offset);
matrix * shmop_deserialise_mat(void * shmaddr, int offset);

#endif // SHMOPS
