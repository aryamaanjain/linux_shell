#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define PATH_LEN 256


int main(int argc, char const *argv[])
{
	pid_t pid;

	if (argc == 1)
		pid = getppid();
	else
		pid = atoi(argv[1]);

	char strpid[128];
	sprintf(strpid, "%d", pid);

	char procStat[128] = "/proc/";
	strcat(procStat, strpid);
	strcat(procStat, "/stat");

	char temp[1024];
	char status[16];
	char vmem[1024];

	FILE *fd = fopen(procStat, "r");
	if (fd == NULL)
	{
		perror("pinfo");
		exit(EXIT_FAILURE);
	}
	
	for(int i=1; i<=23; i++)
	{
		if (i == 3)
			fscanf(fd, "%s", status);	
		else if (i == 23)
			fscanf(fd, "%s", vmem);
		else
			fscanf(fd, "%s", temp);
	}
	fclose(fd);

	char procExe[128] = "/proc/";
	strcat(procExe, strpid);
	strcat(procExe, "/exe");

	char buf[PATH_LEN];
	memset(buf, 0, PATH_LEN);
	readlink(procExe, buf, PATH_LEN);  // buffer has the executable path
	
	char *shellHome;
	shellHome = getenv("home_dir"); // now compare buf with shellHome to print relative path

	char relLoc[PATH_LEN];  // relative location

	if(strncmp(shellHome, buf, strlen(shellHome)) == 0)
	{
		relLoc[0] = '~';
		strncpy(relLoc+1, buf+strlen(shellHome), PATH_LEN-1);
	}
	else
		strncpy(relLoc, buf, PATH_LEN);

	printf("pid: %d\n", pid);
	printf("process status: %s\n", status);
	printf("memory: %s\n", vmem);
	printf("executable path: %s\n", relLoc);

	return 0;
}

