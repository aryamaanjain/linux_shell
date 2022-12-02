#include <stdio.h>
#include <stdlib.h>
#include "shell_struct.h"


int job_stopped (job *j);	// From shell_backbone.c
extern job *first_job;		// From shell_backbone.c


void jobs (char **argv)
{
	job *j = first_job;

	int number = 1;
	while (j)
	{
		fprintf(stdout, "[%d] ", number);
		number++;

		/* Running / Stopped */
		fprintf(stdout, "%s ", job_stopped(j) ? "Stopped" : "Running");

		fprintf(stdout, "%16s ", j->command);
		fprintf(stdout, "[%d]\n", j->pgid);

		j = j->next;
	}

}

