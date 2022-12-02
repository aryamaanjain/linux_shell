#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include "constants.h"
#include "builtin.h"
#include "bglist.h"


void initShell();
void printUSD();  // user system directory
void getCmd();  // get command
void xCmd();  // execute command
void checkBG();
char homeDir[PATH_LEN] = {0};  // home directory


int main()
{
	initShell();

	while (1)
	{
		printUSD();
		getCmd();
		checkBG();
	}

	return 0;
}


void checkBG()
{
	int wstatus;
	pid_t pid;

	while ((pid = waitpid(-1, &wstatus, WNOHANG)) != 0 && pid != -1)
	{
		bgFindDelete(pid);
		fflush(stdout);

		fprintf(stderr, "pid %d exited ", pid);
		
		if (WIFEXITED(wstatus))
			fprintf(stderr, "normally\n");
		else
			fprintf(stderr, "abnormally\n");
	}
}


void initShell()
{
	tcsetpgrp (STDIN_FILENO, getpid());
	
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	
	char cwd[PATH_LEN];
	if ( getcwd(cwd, PATH_LEN) ==  NULL )
		perror("shellHome environment variable set error");
	else
		setenv("shellHome", cwd, 0);

	historyInit();
}


void xCmd(char *command)
{
	char *argv[NO_OF_ARGS];
	int argc = 0;

	char *saveptr;
	argv[argc++] = strtok_r(command, " \t", &saveptr);

	if (strncmp("cd", argv[0], strlen("cd")) == 0)
	{
		argv[argc++] = strtok_r(NULL, " \t&", &saveptr); 
		if (strncmp("~", argv[1], 1) == 0)
			argv[1] = homeDir;
		argv[argc] = 0;
		cd(argc, argv);
	}

	else if (strncmp("pwd", argv[0], strlen("pwd")) == 0)
	{
		argv[argc] = 0;
		pwd(argc, argv);
	}

	else if (strncmp("echo", argv[0], strlen("echo")) == 0)
	{
		argv[argc++] = strtok_r(NULL, "&", &saveptr); 
		argv[argc] = 0;
		echo(argc, argv);
	}

	else if (strncmp("ls", argv[0], strlen("ls")) == 0)
	{
		argv[argc] = strtok_r(NULL, " \t&", &saveptr);
		if (argv[argc] != NULL)
		{
			if (strncmp("~", argv[argc], 1) == 0)
				argv[argc] = homeDir;
			argv[++argc] = 0;
		}
		ls(argc, argv);
	}

	else if (strncmp("history", argv[0], strlen("history")) == 0)
	{
		argv[argc] = strtok_r(NULL, " \t&", &saveptr);
		if (argv[argc] != NULL)
			argv[++argc] = 0;
		history(argc, argv);
	}

	else if (strncmp("exit", argv[0], strlen("exit")) == 0)
	{
		exit(EXIT_SUCCESS);
	}

	else
	{
		if (argv[0] == NULL)
			return;

		while (argv[argc-1] != NULL)
			argv[argc++] = strtok_r(NULL, " \t", &saveptr); 
		argc--;

		int bgProcess = 0;
		if (argv[argc-1][0] == '&')
		{
			bgProcess = 1;
			argv[--argc] = 0;
		}

		pid_t pid = fork();
		if (pid == 0)
		{
			setpgid(0, 0);

			if (bgProcess == 0)
			{
				tcsetpgrp(STDIN_FILENO, getpid());
			}
			
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);
			signal(SIGTTIN, SIG_DFL);
			signal(SIGTTOU, SIG_DFL);
			signal(SIGCHLD, SIG_DFL);

			execvp(argv[0], argv);
			perror ("execvp");
			exit (EXIT_SUCCESS);
		}
		else
		{
			setpgid(pid, pid);
			if (bgProcess == 0)
			{
				tcsetpgrp(STDIN_FILENO, pid);
				waitpid(pid, NULL, 0);
				tcsetpgrp(STDIN_FILENO, getpid());
			}
			else
			{
				bgInsert(pid, argv[0]);
			}
		}
	}

}


void getCmd()
{
	char command[COMMAND_LEN];
	fgets(command, COMMAND_LEN, stdin);
	historyStore(command);
	command[strcspn(command, "\n")] = '\0';

	char *saveptr, *token;
	token = strtok_r(command, ";", &saveptr); 
	while (token != NULL)
	{ 
		xCmd(token);
		token = strtok_r(NULL, ";", &saveptr); 
	}
}


void printUSD()
{
	char curDirFull[PATH_LEN] = {0};  // full current dorectory
	char curDirMod[PATH_LEN] = {0};  // modified current directory
	char hostName[HOSTNAME_LEN] = {0};  // system name

	if(homeDir[0] == 0)		// first call initializes
		getcwd(homeDir, PATH_LEN);

	getcwd(curDirFull, PATH_LEN);

	if(strncmp(homeDir, curDirFull, PATH_LEN) == 0)  // get rid of this if, the next else if will take care of all
		strncpy(curDirMod, "~", PATH_LEN);
	else if(strncmp(homeDir, curDirFull, strlen(homeDir)) == 0)
	{
		curDirMod[0] = '~';
		strncpy(curDirMod+1, curDirFull+strlen(homeDir), PATH_LEN-1);
	}
	else
		strncpy(curDirMod, curDirFull, PATH_LEN);

	gethostname(hostName, HOSTNAME_LEN);

	printf("<%s@%s:%s> ", getlogin(), hostName, curDirMod);
}

