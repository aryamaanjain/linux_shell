#include <stdlib.h>
#include <stdio.h>
#include "shell_struct.h"


void job_resume (job *j, int foreground);		// From shell_backbone.c
extern job *first_job;		// From shell_backbone.c


void fg (char **argv)
{
	if (argv[1] == NULL)
	{
		fprintf(stderr, "fg: Error in arguments.\n");
		return;
	}

	int job_num = atoi (argv[1]);

	int cur_num = 1;
	job *cur = first_job;

	while (cur)
	{
		if (cur_num == job_num)
			break;
		cur_num++;
		cur = cur->next;
	}

	if (cur_num != job_num)
	{
		fprintf(stderr, "Invalid job number.\n");
		return;
	}

	job_resume (cur, 1);
}


void bg (char **argv)
{
	if (argv[1] == NULL)
	{
		fprintf(stderr, "bg: Error in arguments.\n");
		return;
	}

	int job_num = atoi (argv[1]);

	int cur_num = 1;
	job *cur = first_job;

	while (cur)
	{
		if (cur_num == job_num)
			break;
		cur_num++;
		cur = cur->next;
	}

	if (cur_num != job_num)
	{
		fprintf(stderr, "Invalid job number.\n");
		return;
	}

	job_resume (cur, 0);
}

