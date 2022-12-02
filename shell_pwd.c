#include <stdio.h>
#include <unistd.h>


#define PATH_LEN 1024


void pwd(char **argv)
{
	char cwd[PATH_LEN];
	
	if (getcwd(cwd, PATH_LEN) ==  NULL)
	{
		perror("Error pwd");
		return;
	}
	
	printf("%s\n", cwd);
}
