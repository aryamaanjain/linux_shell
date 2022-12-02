#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BS 512  // buffer size


void dirty(time_t);
void interrupt(time_t);


int main(int argc, char const *argv[])
{
	if (argc != 4 || strncmp("-n", argv[1], 2) != 0)
		fprintf(stderr, "nightswatch: error in command line arguments\n");

	else if (strncmp("interrupt", argv[3], strlen("interrupt")) == 0)
		interrupt(atoi(argv[2]));

	else if (strncmp("dirty", argv[3], strlen("dirty")) == 0)
		dirty(atoi(argv[2]));
	
	else
		fprintf(stderr, "nightswatch: error in command line arguments\n");
	
	return 0;
}


void dirty(time_t t)
{
	while (1)
	{
		FILE *fd = fopen("/proc/meminfo", "r");
	
		char s[BS];
	
		for (int i=0; i<17; i++)
			fgets(s, BS, fd);
	
		char *token = strtok(s, " \n");
		token = strtok(NULL, " \n");
		printf("%s ", token);
		token = strtok(NULL, " \n");
		printf("%s\n", token);
	
		fclose(fd);
		sleep(t);
	}
}


void interrupt(time_t t)
{
	FILE *fd = fopen("/proc/interrupts", "r");
	char s[BS];
	int cpuCount = 0;
	fgets(s, BS, fd);
	char *token = strtok(s, " \n");
	while (token != NULL)
	{
		printf("%8s", token);
		token = strtok(NULL, " \n");
		cpuCount++;
	}
	printf("\n");
	fclose(fd);

	while (1)
	{
		fd = fopen("/proc/interrupts", "r");
		fgets(s, BS, fd);
		fgets(s, BS, fd);
		fgets(s, BS, fd);
		char *token = strtok(s, " \n");
		token = strtok(NULL, " \n");
		for (int i = 0; i < cpuCount; i++)
		{
			printf("%8s", token);
			token = strtok(NULL, " \n");
		}
		printf("\n");
		fclose(fd);
		sleep(t);
	}
}


/*
	Bug:
	Redirection will probably not work because ending with ^C.
	Maybe since file handling not proper due to incomplete signal handling.
*/

