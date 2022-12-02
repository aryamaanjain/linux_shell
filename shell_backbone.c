#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "shell_struct.h"
#include "shell_builtin.h"


void initialize_shell ();
int job_stopped (job *j);
int job_complete (job *j);
void launch_process (process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground);
void launch_job (job *j, int foreground);
void job_to_fg (job *j, int cont);
void job_to_bg (job *j, int cont);
int update_process_status (pid_t pid, int status);
void update_job_status (void);
void job_resume (job *j, int foreground);
void free_job (job *j, int print_info);


builtin_function give_builtin_function (char *cstr);	// From shell_builtin.c


job *first_job = NULL;
pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;


void initialize_shell ()
{
	shell_terminal = STDIN_FILENO;

	while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
		kill (- shell_pgid, SIGTTIN);

	signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);

	shell_pgid = getpid ();
	setpgid (shell_pgid, shell_pgid);

	tcsetpgrp (shell_terminal, shell_pgid);
	tcgetattr (shell_terminal, &shell_tmodes);
}


int job_stopped (job *j)
{
	process *p = j->first_process;

	while (p != NULL)
	{
		if (p->completed == 0 && p->stopped == 0)
			return 0;
		else
			p = p->next;
	}
	return 1;

}


int job_complete (job *j)
{
	process *p = j->first_process;

	while (p != NULL)
	{
		if (p->completed == 0)
			return 0;
		else
			p = p->next;
	}
	return 1;
}


void launch_process (process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground)
{
	pid_t pid = getpid ();
	if (pgid == 0) 
		pgid = pid;
	setpgid (pid, pgid);
	if (foreground)
		tcsetpgrp (shell_terminal, pgid);

	signal (SIGINT, SIG_DFL);
	signal (SIGQUIT, SIG_DFL);
	signal (SIGTSTP, SIG_DFL);
	signal (SIGTTIN, SIG_DFL);
	signal (SIGTTOU, SIG_DFL);
	signal (SIGCHLD, SIG_DFL);

	if (infile != STDIN_FILENO)
	{
		dup2 (infile, STDIN_FILENO);
		close (infile);
	}
	if (outfile != STDOUT_FILENO)
	{
		dup2 (outfile, STDOUT_FILENO);
		close (outfile);
	}
	if (errfile != STDERR_FILENO)
	{
		dup2 (errfile, STDERR_FILENO);
		close (errfile);
	}

	/* Check if command is builtin. */
	if ( give_builtin_function (p->argv[0]) )		
	{
		(give_builtin_function (p->argv[0])) (p->argv);
		exit (EXIT_SUCCESS);
	}
	else
	{
		execvp (p->argv[0], p->argv);
		perror ("execvp");
		exit (1);
	}
}


void launch_job (job *j, int foreground)
{
	process *p = j->first_process;
	pid_t pid;
	int mypipe[2], infile, outfile;

	infile = j->stdin;
	while (p != NULL)
	{
		if (p->next != NULL)
		{
			pipe (mypipe);
			outfile = mypipe[1];
		}
		else
			outfile = j->stdout;

		/* Fork the child processes.  */
		pid = fork ();
		if (pid == 0)	/* Child process. */
			launch_process (p, j->pgid, infile, outfile, j->stderr, foreground);
		else			/* Parent process */
		{
			p->pid = pid;
			if (j->pgid == 0)
				j->pgid = pid;
			setpgid (pid, j->pgid);
		}

		if (infile != j->stdin)
			close (infile);
		if (outfile != j->stdout)
			close (outfile);
		
		infile = mypipe[0];

		p = p->next;
	}

	if (foreground == 0)
		job_to_bg (j, 0);
	else
		job_to_fg (j, 0);
}


void job_to_fg (job *j, int cont)
{
	tcsetpgrp (shell_terminal, j->pgid);

	if (cont != 0)
	{
		tcsetattr (shell_terminal, TCSADRAIN, &j->tmodes);
		kill (- j->pgid, SIGCONT);
	}

	int status;
	pid_t pid;
	do
		pid = waitpid (WAIT_ANY, &status, WUNTRACED);
	while (!(update_process_status(pid, status) || job_stopped(j) || job_complete(j)));

	tcsetpgrp (shell_terminal, shell_pgid);
	tcgetattr (shell_terminal, &j->tmodes);
	tcsetattr (shell_terminal, TCSADRAIN, &shell_tmodes);

	if (job_complete(j))	/* Then remove it from job list. */
	{
		if (j == first_job)
			first_job = first_job->next;
		else
		{
			job *prev = first_job;
			while (prev->next != j)
				prev = prev->next;
			prev->next = j->next;
		}
		free_job(j, 0);
	}
}


void job_to_bg (job *j, int cont)
{
	if (cont != 0)
		kill (-j->pgid, SIGCONT);
}


int update_process_status (pid_t pid, int status)
{
	job *j = first_job;
	process *p;

	while (j != NULL)
	{
		p = j->first_process;
		while (p != NULL)
		{
			if (p->pid == pid)
			{
				p->status = status;
				if (WIFSTOPPED (status))
					p->stopped = 1;
				else
					p->completed = 1;
				return 0;
			}
			p = p->next;
		}
		j = j->next;
	}
	return -1;
}


void update_job_status (void)
{
	int status;
	pid_t pid;
	do
		pid = waitpid (WAIT_ANY, &status, WUNTRACED|WNOHANG);
	while (!update_process_status (pid, status));

	job *j = first_job;
	job *jprev = NULL;
	job *jnext;
	process *p;

	while(j != NULL)
	{
		jnext = j->next;	/* Since free(j) might be used and j->next may not be available subsequently. */ 

		/* Already removed if completed as foreground process. So only background and completed process go here. */
		if (job_complete (j))
		{
			if (jprev == NULL)
				first_job = jnext;
			else
				jprev->next = jnext;
			free_job (j, 1);
		}
		else if (job_stopped (j) && !j->notified)
		{
			fprintf (stderr, "\033[1;33m" " Stopped: %s\n" "\033[0m", j->command);
			j->notified = 1;
			jprev = j;
		}
		else
		{
			jprev = j;
		}

		j = jnext;
	}

}


void job_resume (job *j, int foreground)
{
	process *p = j->first_process;
	while (p != NULL)
	{
		p->stopped = 0;
		p = p->next;
	}
	j->notified = 0;

	if (foreground == 0)
		job_to_bg (j, 1);
	else
		job_to_fg (j, 1);
}


void free_job (job *j, int print_info)
{
	// free (command);
	process *p = j->first_process;
	while (p)
	{
		if (print_info)
		{
			if (WIFEXITED(p->status))
				fprintf(stderr, "\033[1;36m" "%s with pid %d exited normally.\n" "\033[0m", p->argv[0], p->pid);
			else
				fprintf(stderr, "\033[1;31m" "%s with pid %d exited abnormally.\n" "\033[0m", p->argv[0], p->pid);
		}

		process *temp = p;
		p = p->next;
		// free (temp->argv);
		free (temp);
	}

	if (j->stdin != STDIN_FILENO)
		close (j->stdin);
	
	if (j->stdout != STDOUT_FILENO)
		close (j->stdout);
	
	if (j->stderr != STDERR_FILENO)
		close (j->stderr);

	free (j);
}

