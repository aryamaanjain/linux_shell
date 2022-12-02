#include <termios.h>


typedef struct process
{
	struct process *next;
	char **argv;
	pid_t pid;
	char completed;
	char stopped;
	int status;
} process;


typedef struct job
{
	struct job *next;
	char *command;
	process *first_process;
	pid_t pgid;
	char notified;
	struct termios tmodes;
	int stdin, stdout, stderr;
} job;

