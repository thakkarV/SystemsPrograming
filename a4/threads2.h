#ifndef THREADS
#define THREADS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <stdbool.h>

typedef struct task
{
	jmp_buf bufjump;
	int tid;
	bool is_done;
	void * output;
	struct task * previous;
	struct task * next;
} task;


// API Interface
void my_thr_create(void (*func) (int), int tid);
void start_threads(void);

// Internal
// Task State Managers
void add_task(task * thread);
void remove_task(task * thread);
void thread_exit(void);

// Premeption
void schedule(void);
void set_alarm(void);
void alarm_handler(int signo);

#endif // THREADS
