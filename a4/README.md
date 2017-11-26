# Assignment 4 - IPC and User Space Threading

## Build
This project required some more serious makefile kung fu. The makefile builds all the 
four required programs for the submission. ./matmult_p is the main for multi-process
matrix multiplication, which fork and execs many ./multiply binaries, each one responsible
for a single cell in the output matrix.

## Comments on the Threading API implementation
As far as I possibly can, I have tried to make the threading API as tight and elegant as possible.
I have not take the liberty of calling schedule at the end of the multiply function. To achieve this,
the my_thr_create function does not set the context to the multiply input function directly,
rather it sets the new context to a wrapper (thread_start) which in turn calls the input function
with the input args. 
For starting the threads, the user just has the call the start_threads function after creating all of them. 
The user also does not have to worry about resetting the SIGALRM handler, as the threading library will restore the orginal signal handler before it returns control to the user.
All of this results in an API that does not require the user to write their code arround the the API. The API can truely therefore provide an opaque interface.

## What is working
1. Multi-process seems to be working just fine, to the limits of my initial testing.
2. Matformatter is fine and dandy.
3. Multiply works fine too.
4. Preemption for user space threading

## What is not working
Multi threaded part sometimes works and sometimes does not. This only happend on some compsuters. The issues are mainly regarding the signal handling. Sometimes the handler is registered succesfully, but other times, it neither causes a perror to trigger, nor does it catch the SIGALRM. I have not been able to figure out why this happens. Preemption seems to be working when it does.

As a result of this issue, sometimes ./matmul_t needs to be just re-run (without even recompiling)
once or twice before it outputs the matrix, rather than just "Alarm Clock". I have increased the alarm time up to 50000 microseconds for now, and that seems to make it work consistently.

Update: This is not a problme on the CSA VMs for some reason, only on the Eng-Grid, which also run Cent-OS.
