#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#define PATH_LEN 1024
#define COMMAND_LEN 256


void parse_command (char *command);		// From shell_command_parser.c
void update_job_status (void);	// From shell_backbone.c
void initialize_shell ();		// From shell_backbone.c
void prompt ();		// From shell_prompt.c
void historyInit();		//  From shell_history.c
void historyStore(char *command);	//  From shell_history.c
void historyUpArrow(char *command);		//  From shell_history.c
void set_home_dir ();


char home_dir[PATH_LEN];


int main(int argc, char const *argv[])
{
	char command[COMMAND_LEN];
	initialize_shell ();
	set_home_dir ();
	historyInit();

	while (1)
	{
		prompt ();
		
		fgets(command, COMMAND_LEN, stdin);

		if (command[0] == 27)		// Up arrow key. ASCII = 27 91 65 => strlen(UP) = 3.
		{
			historyUpArrow (command);
			prompt();
			printf("%s", command);
		}

		historyStore (command);
		command[strcspn(command, "\n")] = '\0';

		parse_command(command);
		update_job_status();
	}
	
	return 0;
}


void set_home_dir ()
{
	getcwd(home_dir, PATH_LEN);
	setenv("home_dir", home_dir, 0);
}

