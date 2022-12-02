#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "shell_struct.h"


extern job *first_job;		// From shell_backbone.c


void kjob (char **argv)
{
	if (argv[1] == NULL || argv[2] == NULL)
		fprintf(stderr, "Error in arguments to kjob.\n");

	int job_num = atoi (argv[1]);
	int sig_num = atoi (argv[2]);

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

	if ( killpg(cur->pgid, sig_num) == -1 )
		perror("Error in kjob killpg");

}

