#include <stdio.h>
#include <unistd.h>


void cd (char **argv)
{
	if ( argv[1] == NULL )
	{
		fprintf(stderr, "Error cd: insuffient arguments\n");
		return;
	}

	if ( chdir(argv[1]) == -1 )
	{	
		perror("Error cd"); 
	}
}

