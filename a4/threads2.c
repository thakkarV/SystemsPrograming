#include "threads.h"

task * task_list;
task * current;
ucontext_t * exit_context;
ucontext_t * main_context;

// 4 kib stack per thread
#define STACK_SIZE 4096

void my_thr_create(mctx_t * mctx, void (*func)(void), void *sf_arg, void *sk_addr, size_t sk_size)
{
	struct sigaction sa;
	struct sigaction osa;
	struct sigaltstack ss;
	struct sigaltstack oss;
	sigset_t osigs;
	sigset_t sigs;

	/* Step 1: */
	sigemptyset(&sigs);
	sigaddset(&sigs, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigs, &osigs);

	/* Step 2: */
	memset((void *)&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = mctx_create_trampoline;
	sa.sa_flags = SA_ONSTACK;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, &osa);
	
	/* Step 3: */
	ss.ss_sp = sk_addr;
	ss.ss_size = sk_size;
	ss.ss_flags = 0;
	sigaltstack(&ss, &oss);
	
	/* Step 4: */
	mctx_creat = mctx;
	mctx_creat_func = sf_addr;
	mctx_creat_arg = sf_arg;
	mctx_creat_sigs = osigs;
	mctx_called = FALSE;
	kill(getpid(), SIGUSR1);
	sigfillset(&sigs);
	sigdelset(&sigs, SIGUSR1);
	while (!mctx_called)
		sigsuspend(&sigs);
	
	/* Step 6: */
	sigaltstack(NULL, &ss);
	ss.ss_flags = SS_DISABLE;
	sigaltstack(&ss, NULL);
	if (!(oss.ss_flags & SS_DISABLE))
		sigaltstack(&oss, NULL);
	sigaction(SIGUSR1, &osa, NULL);
	sigprocmask(SIG_SETMASK, &osigs, NULL);
	
	/* Step 7 & Step 8: */
	mctx_switch(&mctx_caller, mctx);

}

void mctx_create_trampoline(int sig)
{
	if (mctx_save(mctx_creat) == 0)
	{
		mctx_called = TRUE;
		return;
	}
	mctx_create_boot();
}

void mctx_create_boot(void)
{
	void (*mctx_start_func)(void *);
	void * mctx_start_arg;
	sigprocmask(SIG_SETMASK, &mctx_creat_sigs, NULL);
	mctx_start_func = mctx_creat_func;
	mctx_start_arg = mctx_creat_arg;
	mctx_switch(mctx_creat, &mctx_caller);
	mctx_start_func(mctx_start_arg);
	abort();
}

void start_threads(void)
{
	// register the signal handler first for alarm
	struct sigaction default_action;
	struct sigaction alarm_action;

	alarm_action.sa_handler = alarm_handler;
	alarm_action.sa_sigaction = NULL;
	sigemptyset(&alarm_action.sa_mask);
	alarm_action.sa_flags = 0;

	if (sigaction(SIGALRM, &alarm_action, &default_action) < 0)
		perror("sigaction");

	current = task_list;

	// store context of main
	main_context = malloc(sizeof(ucontext_t));
	getcontext(main_context);

	// keep scheduling threads until all are done
	while (task_list)
	{
		schedule();
		remove_task(current);
		swapcontext(main_context, &(current-> context));
	}

	// dealloc all tasks here
}

void schedule(void)
{
	task * next = current-> next;
	current = next;
}

void set_alarm(void)
{
	ualarm(20, 0);
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
		task_list-> previous -> next = thread;
		task_list = thread;
	}
	printf("Added task\n");
}

void remove_task(task * thread)
{
	if (thread-> next == thread)
	{
		// dealloc
		free(thread-> context.uc_stack.ss_sp);
		free(thread);
		free(task_list);
	}
	else
	{
		current = current-> next;
		// splice out
		thread-> previous-> next = thread-> next;
		thread-> next-> previous = thread-> previous;
	}
}

void alarm_handler(int signo)
{
	// setcontext(current-> context)
}

void thread_exit(void)
{
	// called automatically when a thread's runnable returns
	// removes the thread form the task list, and schedules the next one
	current-> is_done = true;
	remove_task(current);
	setcontext(main_context);
}
