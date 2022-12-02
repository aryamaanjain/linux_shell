#include <unistd.h>
#include <stdio.h>
#include <string.h>


#define HOSTNAME_LEN 128
#define PATH_LEN 1024
#define BGREN "\033[1;32m"
#define BBLUE "\033[1;34m"
#define RESET "\033[0m"



extern char home_dir[PATH_LEN];		// From shell_main.c


void prompt ()
{
	char cur_dir_full[PATH_LEN] = {0};  // full current directory
	char cur_dir_mod[PATH_LEN] = {0};  // modified current directory
	char host_name[HOSTNAME_LEN] = {0};  // system name

	getcwd(cur_dir_full, PATH_LEN);

	if(strncmp(home_dir, cur_dir_full, PATH_LEN) == 0)  // get rid of this if, the next else if will take care of all
	{
		strncpy(cur_dir_mod, "~", PATH_LEN);
	}
	
	else if(strncmp(home_dir, cur_dir_full, strlen(home_dir)) == 0)
	{
		cur_dir_mod[0] = '~';
		strncpy(cur_dir_mod+1, cur_dir_full+strlen(home_dir), PATH_LEN-1);
	}
	
	else
	{
		strncpy(cur_dir_mod, cur_dir_full, PATH_LEN);
	}
	gethostname(host_name, HOSTNAME_LEN);

	printf("\n<" BGREN "%s@%s" RESET ":" BBLUE "%s" RESET "> ", getlogin(), host_name, cur_dir_mod);
}

