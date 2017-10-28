#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h> // waitpid
#include "exector.h"

const char * prompt_str = "myshell$> ";

void execute(process ** exec_list)
{
	int counter = 0;
	process * p;
	while ((p = exec_list[counter] != NULL))
	{
		int status;
		pid_t pid;
		pid_t wpid;

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
					printf("%s: No such file or directory\n", );
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
				dup2(fid_err, 0);
				close(fid_err);
			}

			// now exec
			execvp(p->argv[0], p-> argv);

			exit(EXIT_FAILFURE); // because if we ever reach here, the child was a stillborn
		}
	}
}

void prompt(void)
{
	printf("%s", prompt_str);
}
