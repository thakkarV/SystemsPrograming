#ifndef LOCK
#define LOCK

#include <stdbool.h>

struct my_lock
{
	bool is_taken;
	int 
};

void initialize_lock(struct my_lock * lock);
void lock(struct my_lock * lock);
void unlock(struct my_lock * lock);

#endif // LOCK
