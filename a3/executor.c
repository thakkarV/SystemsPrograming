#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h> // waitpid
#include <unistd.h> // fork
#include <sys/stat.h> // open
#include <sys/types.h>
#include <fcntl.h>
#include "executor.h"

void execute(process ** exec_list)
{
	int counter = 0;
	process * p;
	while ((p = exec_list[counter++]) != NULL)	
	{
		int status;
		pid_t pid;
		pid_t wpid;
		int fids[2];
		pid_t pids[2];

		if (p-> is_piped_next)
		{
			int pipe_status = pipe(fids);
			// hande write end of pipe
			if ((pids[0] = fork()) > 0) // PARENT
			{
				// do nothing
			}
			else // CHILD
			{
				// we have to map primary processe's stdout
				dup2(fids[1], 1);
				close(fids[1]);
				execvp(p-> argv[0], p-> argv);
			}

			// handle write end of pipe
			if ((pids[1] = fork()) > 0) // PARENT
			{
				if (!(p-> next-> is_background))
				{
					do
					{
						wpid = waitpid(pids[1], &status, WUNTRACED);
					} while (!WIFEXITED(status) && !WIFSIGNALED(status));
				}
			}
			else // CHILD
			{
				dup2(fids[0], 0);
				close(fids[0]);
				execvp(p-> next-> argv[0], p-> next-> argv);
			}
		}
		else
		{
			if ((pid = fork()) > 0) // PARENT
			{
				if (!(p->is_background))
				{
					// wait until the child either returns normally or if it is killed through a signal
					do
					{
						wpid = waitpid(pid, &status, WUNTRACED);
					} while (!WIFEXITED(status) && !WIFSIGNALED(status));
				}
				
			}
			else // CHILD
			{
				// error in fork if return is -1
				if (pid == -1)
					perror("fork");

				// Hande stdin redirection
				if (p-> f_stdin)
				{
					// open file
					int fid_in = open(p-> f_stdin, O_RDONLY, S_IRWXU);
					if (fid_in == -1)
					{
						prompt();
						printf("%s: No such file or directory\n", p-> f_stdin);
					}

					dup2(fid_in, 0);
					close(fid_in);
				}

				// Hande stdout redirection
				if (p-> f_stdout)
				{
					// open file, create if not there
					int fid_out = open(p->f_stdout, O_WRONLY | O_CREAT, S_IRWXU);
					if (fid_out == -1)
						perror("Could not open file.");

					dup2(fid_out, 1);
					close(fid_out);
				}

				// Hande stderr redirection
				if (p-> f_stderr)
				{
					// open file, create if not there
					int fid_err = open(p->f_stderr, O_WRONLY | O_CREAT, S_IRWXU);
					if (fid_err == -1)
						perror("Could not open file.");
					dup2(fid_err, 2);
					close(fid_err);
				}

				// now exec
				execvp(p->argv[0], p-> argv);

				exit(EXIT_FAILURE); // because if we ever reach here, the child was a stillborn
			}
		}
	}
}

void prompt(void)
{
	printf("%s", "myshell$> ");
}
