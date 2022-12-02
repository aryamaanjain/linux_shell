typedef void (*no_fork_builtin_function) (char **);


no_fork_builtin_function give_no_fork_builtin_function (char *cstr);


void quit (char **argv);
void fg (char **argv);
void bg (char **argv);
void cd (char **argv);
void ssetenv (char **argv);
void sunsetenv (char **argv);

