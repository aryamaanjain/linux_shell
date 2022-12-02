#include <string.h>
#include "shell_no_fork_builtin.h"


#define N 6	// Number of builtin commands. 
#define BUILTIN_NAME_LEN 256


typedef struct builtin_dict
{
	char name[BUILTIN_NAME_LEN];
	no_fork_builtin_function func;
} builtin_dict;


/* Function takes in builtin function name as char string (cstr).
	Returns function pointer if exists.
	If builting doesn't exist then returns NULL. 

	For adding additional builtins:
	1) Change the value of N (#define).
	2) Add entry in dict (in give_builtin_function). 
	3) Add entry in header file. */


/*	Checks for builtins which cannot be forked. 
	They have no input/output/pipe.
	They cannot be in process group. */

no_fork_builtin_function give_no_fork_builtin_function (char *cstr)
{
	builtin_dict dict[N] = 
	{
		{ "quit", quit },
		{ "fg", fg },
		{ "bg", bg },
		{ "cd", cd },
		{ "setenv", ssetenv },
		{ "unsetenv", sunsetenv }
	};


	for (int i=0; i<N; i++)
		if ( strcmp(dict[i].name, cstr) == 0 )
			return dict[i].func;
	return NULL;
}

