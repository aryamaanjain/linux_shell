# Linux Shell
Aryamaan Jain

An alternate linux shell. A brief description of all files including what specifications they correspond to are listed below.

To compile and execute
`	$ make
	$ ./shell
	<aryam@comp:~> commands
	<aryam@comp:~> quit
	$`
_________________________________________________________

* makefile
Command: $ make
To clean: $ make clean
_________________________________________________________

* shell_backbone.c
Contains the logic of shell working.
Specification 1: Foreground and background processes
Specification 2: Implement input-output redirection functionality
Specification 3: Implement command redirection using pipes
Specification 4: Implement i/o redirection + pipes redirection
_________________________________________________________

* shell_builtin.c
Managing built-in commands that fork.
_________________________________________________________

* shell_builtin.h
Header file containing all built-in function prototypes of functions that fork.
_________________________________________________________

* shell_cd.c
Assignment 2 requirement.
_________________________________________________________

* shell_command_parser.c
Takes the input command, parses it and stores it in the job and process structures.
Specification 1: Foreground and background processes
Specification 2: Implement input-output redirection functionality
Specification 3: Implement command redirection using pipes
Specification 4: Implement i/o redirection + pipes redirection

_________________________________________________________

* shell_env.c
Specification 5: User-defined commands.
Contains code for setenv and unsetenv.
_________________________________________________________

* shell_fg_bg.c
Specification 5: User-defined commands.
Contains code for fg and bg.
_________________________________________________________

* shell_history.c
Command Recall using ‘UP’ arrow key.
Continued from assignment 2.
Added extra features to previous module to support new functionalities.
_________________________________________________________

* shell_jobs.c
Specification 5: User-defined commands.
Contains code for jobs command.
_________________________________________________________

* shell_kjob.c
Specification 5: User-defined commands.
Contains code for kjob command.
_________________________________________________________

* shell_ls.c
Assignment 2 requirement.
_________________________________________________________

* shell_main.c
Contains main() of the shell.
Co-ordinates the functionality by calling functions from different files.
_________________________________________________________

* shell_no_fork_builtin.c
Managing built-in commands that don't fork.
_________________________________________________________

* shell_no_fork_builtin.h
Header file containing all built-in function prototypes of functions that don't fork.
_________________________________________________________

* shell_overkill.c
Specification 5: User-defined commands.
Contains code for overkill command.
_________________________________________________________

* shell_prompt.c
Contains code for shell prompt.

_________________________________________________________

* shell_quit.c
Specification 5: User-defined commands.
Contains code for quit command.
_________________________________________________________

* shell_struct.h
Contains structures used in shell.
_________________________________________________________
