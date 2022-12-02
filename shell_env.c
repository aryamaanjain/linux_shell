/*
	Prefix 's' is used in function names because setenv and unsetenv are pre defined functions in stdlib.h.
*/


#include <stdlib.h>
#include <stdio.h>


void ssetenv (char **argv)
{
	if (argv[1] == NULL || argv[3] != NULL)
	{
		fprintf(stderr, "setenv: Error in command line arguments.\n");	
		return;
	}

	if ( argv[2] == NULL )
		setenv(argv[1], "", 1);
	else
		setenv(argv[1], argv[2], 1);
}


void sunsetenv(char **argv)
{
	if (argv[1] == NULL)
	{
		fprintf(stderr, "unsetenv: Error in command line arguments.\n");	
		return;
	}

	unsetenv(argv[1]);
}

