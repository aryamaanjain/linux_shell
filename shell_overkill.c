#include <signal.h>
#include <stdio.h>
#include "shell_struct.h"


extern job *first_job;	// From shell_backbone.c

/* Double check on how global data structures are affected after this.
	This might give more insight upon left over corner cases. */

void overkill (char **argv)
{
	job *cur = first_job;
	while (cur->next)	// Not till (cur) since last job will be overkill itself.
	{
		if ( killpg(cur->pgid, SIGKILL) == -1 )
			perror("Error in kjob killpg");
		cur = cur->next;
	}

}

