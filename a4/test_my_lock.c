#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lock.h"

#define NUM_THREADS 5

int global = 0;
struct my_lock testlock;

void *increment(void *threadid)
{
	int i = 0;
	for(i = 0; i < 1000000; i++)
	{
		lock(&testlock);
		global++;
		unlock(&testlock);
	}

	pthread_exit(NULL);
}

int main(int argc, char * argv [])
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long i;

	initialize_lock(&testlock);

	for(i = 0; i < NUM_THREADS; i++)
	{
		printf("In main: creating thread %ld\n", i);
		rc = pthread_create(&threads[i], NULL, increment, (void *)i);

		if(rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for(i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}

	printf("global = %d\n", global);

	/* Last thing that main() should do */
	pthread_exit(NULL);
}
