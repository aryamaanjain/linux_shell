#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "constants.h"


void cd(int argc, char *argv[])
{
	if (chdir(argv[1]) == -1)
		perror("error! cd"); 
}


void pwd(int argc, char *argv[])
{
	char cwd[PATH_LEN];
	
	if (getcwd(cwd, PATH_LEN) ==  NULL)
	{
		perror("error! pwd");
		return;
	}
	
	printf("%s\n", cwd);
}


void echo(int argc, char *argv[])
{
	char *str = argv[1];

	while (*str == ' ' || *str == '\t')
		str++;

	printf("%s\n", str);
}

