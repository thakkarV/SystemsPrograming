#ifndef SHMOPS
#define SHMOPS

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "datastructures.h"

void shmop_serialize_mat(const void * shmaddr, const matrix * mat, int offset);
matrix * shmop_deserialize_mat(const void * shmaddr, int offset);

#endif // SHMOPS
