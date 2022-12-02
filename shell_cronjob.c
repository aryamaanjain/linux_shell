#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


/* Did not think much in this command.
	Probably has some bugs like 
		maybe handling zombies, 
		when control of process group returns due to additional processes forked,
		etc.
	Use less. */
void cronjob (char **argv)
{
	int i = 1;
	int t = 0, p = 0;
	char *command;

	while ( argv[i] != NULL )
	{
		if ( strcmp(argv[i], "-c") == 0 )
		{
			i++;
			if ( argv[i] == NULL )
			{
				fprintf(stderr, "cronjob: Error in arguments.\n");
				return;		
			}
			command = argv[i];
		}

		else if ( strcmp(argv[i], "-t") == 0 )
		{
			i++;
			if ( argv[i] == NULL )
			{
				fprintf(stderr, "cronjob: Error in arguments.\n");
				return;		
			}

			t = atoi(argv[i]);
		
			if (t == 0)
			{
				fprintf(stderr, "cronjob: Error in argument -t.\n");
				return;
			}
		}

		else if ( strcmp(argv[i], "-p") == 0 )
		{
			i++;
			if ( argv[i] == NULL )
			{
				fprintf(stderr, "cronjob: Error in arguments.\n");
				return;
			}

			p = atoi(argv[i]);
			
			if (p == 0)
			{
				fprintf(stderr, "cronjob: Error in argument -p.\n");
				return;
			}
		}

		else
		{
			fprintf(stderr, "cronjob: Error in arguments.\n");
			return;
		}

		i++;
	}

	if (t == 0 || p == 0)
	{
		fprintf(stderr, "cronjob: Error in argument.\n");
		return;
	}

	int ctr = 0;
	int proc_count = 0;
	while (ctr < p)
	{
		if (fork() == 0)
		{
			execlp (command, command, NULL);
			perror ("execlp");
			exit (1);
		}

		sleep(t);

		proc_count++;
		ctr += t;
	}

	/* Reaping child processes. */
	for (int i=0; i<proc_count; i++)
		wait(NULL);
	
}

