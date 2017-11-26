#include "threads.h"

task * task_list;
task * current;
ucontext_t * exit_context;
ucontext_t * main_context;

// 4 kib stack per thread
#define STACK_SIZE 4096

void my_thr_create(void (*func) (int), int tid)
{
	// allocates new stack space and sets up the context for each new thread
	task * thread = malloc(sizeof(task));
	getcontext(&(thread-> context));

	thread-> context.uc_link = exit_context;
	thread-> context.uc_stack.ss_size = STACK_SIZE;
	thread-> context.uc_stack.ss_sp = malloc(STACK_SIZE);
	thread-> context.uc_stack.ss_flags = 0;
	makecontext(&(thread-> context), (void (*) (void)) thread_runner, 2, func, tid);
	add_task(thread);
}

void start_threads(void)
{
	// register the signal handler first for alarm
	struct sigaction default_action;
	struct sigaction alarm_action;

	memset(&alarm_action, 0, sizeof(sigaction));
	alarm_action.sa_handler = alarm_handler;
	// alarm_action.sa_sigaction = NULL;
	// sigemptyset(&alarm_action.sa_mask);
	// alarm_action.sa_flags = 0;

	if (sigaction(SIGALRM, &alarm_action, &default_action) < 0)
		perror("sigaction");

	current = task_list;

	// store context of main
	main_context = malloc(sizeof(ucontext_t));

	// keep scheduling threads until all are done
	while (task_list)
	{
		set_alarm();
		swapcontext(main_context, &(current-> context));
		schedule();
	}

	// dealloc all tasks here
}

void schedule(void)
{
	current = current-> next;
}

void set_alarm(void)
{
	ualarm(5000, 0);
}

void add_task(task * thread)
{
	if (!task_list)
	{
		task_list = thread;
		thread-> next = thread;
		thread-> previous = thread;
	}
	else
	{
		// insert at the front of the circular linked list
		thread-> next = task_list;
		thread-> previous = task_list-> previous;
		task_list-> previous-> next = thread;
		task_list-> previous = thread;
		current = thread;
	}
}

void remove_task(task * thread)
{
	if (thread-> next == thread)
	{
		// dealloc
		free(thread-> context.uc_stack.ss_sp);
		free(thread);
		free(task_list);
		task_list = NULL;
	}
	else
	{
		// splice out
		thread-> previous-> next = thread-> next;
		thread-> next-> previous = thread-> previous;
	}
}

void alarm_handler(int signo)
{
	printf("In Handler\n");
	swapcontext(&(current-> context), main_context);
}

void thread_runner(void (*func) (int), int tid)
{
	// called automatically when a thread's runnable returns
	// removes the thread form the task list, and schedules the next one
	func(tid);
	sleep(1);
	current-> is_done = true;
	remove_task(current);
	setcontext(main_context);
}
