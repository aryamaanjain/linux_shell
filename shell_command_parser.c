#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "shell_struct.h"
#include "shell_no_fork_builtin.h"
#define MAX_ARGC 128


void parse_command (char *command);
void add_job_to_global_list (job *j);
void add_process_to_job (process *p, job *j);
void intermidiate_error_handle (job *j);
process *new_process ();
job *new_job (char *command);


void launch_job (job *j, int foreground);	// From shell_backbone.c
extern job *first_job;		// From shell_backbone.c
extern struct termios shell_tmodes;		// From shell_backbone.c


/* Take in command entered in shell.
	Return job structure converted from command string. */

void parse_command (char *command)
{
	/* Empty line. */
	if (command[0] == '\0')
		return;

	int foreground = 1;

	job *j = new_job(command);


	char *saveptr;
	char *token = strtok_r(command, " \t", &saveptr); 
	
	while (token != NULL)
	{
		if (strcmp(token, "&") == 0)
			foreground = 0;	


		else if (strcmp(token, "<") == 0)
		{
			token = strtok_r(NULL, " \t", &saveptr);	// Token after "<" is filename.			
			j->stdin = open(token, O_RDONLY);

			if (j->stdin == -1)
			{
				perror("Error opening input file");
				intermidiate_error_handle(j);
				return;
			}
		}

		else if (strcmp(token, ">") == 0)
		{
			token = strtok_r(NULL, " \t", &saveptr);	// Token after ">" is filename.			
			j->stdout = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);

			if (j->stdout == -1)
			{
				perror("Error opening output file");
				intermidiate_error_handle(j);
				return;
			}
		}

		else if (strcmp(token, ">>") == 0)
		{
			token = strtok_r(NULL, " \t", &saveptr);	// Token after ">>" is filename.			
			j->stdout = open(token, O_WRONLY | O_CREAT | O_APPEND, 0644);

			if (j->stdout == -1)
			{
				perror("Error opening output file");
				intermidiate_error_handle(j);
				return;
		
			}
		}

		else if (strcmp(token, "|") == 0)
		{
			// Do nothing.
		}

		else	// First process.
		{
			process *p = new_process();

			/* Fill up argv. */
			p->argv[0] = malloc(sizeof(char) * strlen(token));
			strcpy(p->argv[0], token);		// Command name.

			int argi = 1;	// Keeping track of next argv to fill.
			token = strtok_r(NULL, " \t", &saveptr);
			while (token != NULL && !strchr("<>&|", token[0]))
			{
				p->argv[argi] = malloc(sizeof(char) * strlen(token));
				strcpy(p->argv[argi], token);
				argi++;
				token = strtok_r(NULL, " \t", &saveptr);
			}
			p->argv[argi] = NULL;

			add_process_to_job(p, j);

			continue;	// Have already done tokenization.
		}

		token = strtok_r(NULL, " \t", &saveptr); 
	}

	/* Commands like cd might have redirection but not implemented here.
		no_fork_builtin commands don't support pipes and redirections. */
	if ( give_no_fork_builtin_function(j->first_process->argv[0]) )
	{
		give_no_fork_builtin_function(j->first_process->argv[0]) (j->first_process->argv);
		// free_job(j);
		return;
	}


	/* Add job to global list.
		Think where to remove from list. Complete free_job(). */
	add_job_to_global_list(j);	

	launch_job(j, foreground);
}


void add_job_to_global_list (job *j)
{
	if (first_job == NULL)
	{
		first_job = j;
	}
	else
	{
		job *cur = first_job;
		while (cur->next)
			cur = cur->next;
		cur->next = j;
	}
}


void add_process_to_job (process *p, job *j)
{
	if (j->first_process == NULL)
	{
		j->first_process = p;
	}

	else
	{
		process *cur = j->first_process;
		while (cur->next)
			cur = cur->next;
		cur->next = p;
	}
}


void intermidiate_error_handle (job *j)
{
	free(j);
	/* Add more free()'s for processes, close fd's, etc, if possible. */ 
}


process *new_process ()
{
	process *p = malloc(sizeof (process));

	p->next = NULL;
	
	p->argv = malloc(sizeof(char *) * MAX_ARGC);		// Assuming maximum number of arguments to command is MAX_ARGC.
	p->pid = 0;
	p->completed = 0;
	p->stopped = 0;
	p->status = 0;	// Initialization does not matter.

	return p;
}


job *new_job (char *command)
{
	job *j = malloc(sizeof (job));

	j->next = NULL;

	j->command = malloc( sizeof(char) * (strlen(command) + 1) );
	strncpy (j->command, command, strlen(command)+1);

	j->first_process = NULL;
	j->pgid = 0;
	j->notified = 0;
	j->tmodes = shell_tmodes;
	j->stdin = STDIN_FILENO;
	j->stdout = STDOUT_FILENO;
	j->stderr = STDERR_FILENO;

	return j;
}

