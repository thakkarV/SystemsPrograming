#ifndef THREADS
#define THREADS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <ucontext.h>

#define ALARM_OFFSET 50000
#define ALARM_INTERVAL 50000
// 4 kib stack per thread
#define STACK_SIZE 4096

typedef struct task
{
	ucontext_t context;
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
// void thread_exit(void);
void thread_runner(void (*func) (int), int tid);


// Premeption
void schedule(void);
void set_alarm(void);
void alarm_handler(int signo);

#endif // THREADS
