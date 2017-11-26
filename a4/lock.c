#include "lock.h"

void initialize_lock(struct my_lock * lock)
{
	lock-> is_taken = malloc(sizeof(int));
	*(lock-> is_taken) = 0;
}

void lock(struct my_lock * lock)
{
	while (__sync_bool_compare_and_swap(lock-> is_taken, 0, 1));
}

void unlock(struct my_lock * lock)
{
	lock-> is_taken = 0;
}
