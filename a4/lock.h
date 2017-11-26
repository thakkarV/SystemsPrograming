#ifndef LOCK
#define LOCK

#include <stdlib.h>

struct my_lock
{
	int * is_taken;
};

void initialize_lock(struct my_lock * lock);
void lock(struct my_lock * lock);
void unlock(struct my_lock * lock);

#endif // LOCK
