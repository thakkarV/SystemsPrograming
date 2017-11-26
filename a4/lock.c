#include "lock.h"

void initialize_lock(struct my_lock * lock)
{
	lock = malloc(sizeof(struct my_lock));
}

void lock(struct my_lock * lock)
{
	
}

void unlock(struct my_lock * lock);
