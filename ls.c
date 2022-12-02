#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>


void ls(int argc, char *argv[]);
int filter(const struct dirent *);
void printFileMode(int);
void ls_dir(char *);
void ls_al();
void ls_l();
void ls_a();
void ls_();


void ls(int argc, char *argv[])
{	
	if (argc == 1)
		ls_();
	else if (strncmp("-a", argv[1], 4) == 0)
		ls_a();
	else if (strncmp("-l", argv[1], 4) == 0)
		ls_l();
	else if (strncmp("-al", argv[1], 4) == 0 || strncmp("-la", argv[1], 4) == 0)
		ls_al();
	else
		ls_dir(argv[1]);
}


int filter(const struct dirent *sd)
{
	return strcmp(".", sd->d_name) && strcmp("..", sd->d_name); 
}


void printFileMode(int m)
{
	if      (S_ISREG(m)) 	printf("-");
	else if (S_ISDIR(m)) 	printf("d");	// (directory)
	else if (S_ISCHR(m)) 	printf("c");	// (character device)
	else if (S_ISLNK(m)) 	printf("l");	// (symlink)
	else if (S_ISFIFO(m))	printf("p");	// (named pipe)
	else if (S_ISSOCK(m))	printf("s");	// (socket)
	else if (S_ISBLK(m)) 	printf("b");	// (block device)

	m & S_IRUSR ? printf("r") : printf("-");		// owner has read permission
	m & S_IWUSR ? printf("w") : printf("-");		// owner has write permission
	m & S_IXUSR ? printf("x") : printf("-");		// owner has execute permission

	m & S_IRGRP ? printf("r") : printf("-");		// group has read permission
	m & S_IWGRP ? printf("w") : printf("-");		// group has write permission
	m & S_IXGRP ? printf("x") : printf("-");		// group has execute permission
	
	m & S_IROTH ? printf("r") : printf("-");		// others have read permission
	m & S_IWOTH ? printf("w") : printf("-");		// others have write permission
	m & S_IXOTH ? printf("x") : printf("-");		// others have execute permission
}


void ls_dir(char *dir)
{
	struct dirent **namelist;
	int n = scandir(dir, &namelist, filter, alphasort);
	
	if (n == -1)
	{	
		perror("error! ls");
		return;
	}

	for(int i=0; i<n; i++)
	{
		printf("%s\n", namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);
}


void ls_al()
{
	struct dirent **namelist;
	int n = scandir(".", &namelist, NULL, alphasort);
	
	int total = 0;

	if (n == -1)
	{	
		perror("error! ls");
		return;
	}

	struct stat statbuf;
	char timestr[200];

	for(int i=0; i<n; i++)
	{
		stat(namelist[i]->d_name, &statbuf);

		strftime(timestr, sizeof(timestr), "%b %d %H:%M", localtime(&statbuf.st_mtime));
		
		struct passwd *usr = getpwuid(statbuf.st_uid);
		struct group  *grp = getgrgid(statbuf.st_gid);
		
		printFileMode(statbuf.st_mode);

		printf("%4lu %8s %8s %8ld %s %s\n",statbuf.st_nlink ,usr->pw_name, grp->gr_name,statbuf.st_size, timestr, namelist[i]->d_name);
		
		total += statbuf.st_blocks;

		free(namelist[i]);
	}
	free(namelist);

	printf("total %d\n", total/2);
}


void ls_l()
{
	struct dirent **namelist;
	int n = scandir(".", &namelist, filter, alphasort);
	
	int total = 0;

	if (n == -1)
	{	
		perror("error! ls");
		return;
	}

	struct stat statbuf;
	char timestr[200];

	for(int i=0; i<n; i++)
	{
		stat(namelist[i]->d_name, &statbuf);

		strftime(timestr, sizeof(timestr), "%b %d %H:%M", localtime(&statbuf.st_mtime));
		
		struct passwd *usr = getpwuid(statbuf.st_uid);
		struct group  *grp = getgrgid(statbuf.st_gid);
		
		printFileMode(statbuf.st_mode);

		printf("%4lu %8s %8s %8ld %s %s\n",statbuf.st_nlink ,usr->pw_name, grp->gr_name,statbuf.st_size, timestr, namelist[i]->d_name);
		
		total += statbuf.st_blocks;

		free(namelist[i]);
	}
	free(namelist);

	printf("total %d\n", total/2);
}


void ls_a()
{
	struct dirent **namelist;
	int n = scandir(".", &namelist, NULL, alphasort);
	
	if (n == -1)
	{	
		perror("error! ls");
		return;
	}

	for(int i=0; i<n; i++)
	{
		printf("%s\n", namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);
}


void ls_()
{
	struct dirent **namelist;
	int n = scandir(".", &namelist, filter, alphasort);
	
	if (n == -1)
	{	
		perror("error! ls");
		return;
	}

	for(int i=0; i<n; i++)
	{
		printf("%s\n", namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);
}

