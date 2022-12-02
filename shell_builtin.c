#include <string.h>
#include "shell_builtin.h"


#define N 8	// Number of builtin commands. 
#define BUILTIN_NAME_LEN 256


typedef struct builtin_dict
{
	char name[BUILTIN_NAME_LEN];
	builtin_function func;
} builtin_dict;


/* Function takes in builtin function name as char string (cstr).
	Returns function pointer if exists.
	If builting doesn't exist then returns NULL. 

	For adding additional builtins:
	1) Change the value of N (#define).
	2) Add entry in dict (in give_builtin_function). 
	3) Add prototype in shell_builtin.h */

builtin_function give_builtin_function (char *cstr)
{
	builtin_dict dict[N] = 
	{
		{ "overkill", overkill },
		{ "jobs", jobs },
		{ "kjob", kjob },
		{ "echo", echo },
		{ "pwd", pwd },
		{ "ls", ls },
		{ "history", history },
		{ "cronjob", cronjob }
	};


	for (int i=0; i<N; i++)
		if ( strcmp(dict[i].name, cstr) == 0 )
			return dict[i].func;
	return NULL;
}

