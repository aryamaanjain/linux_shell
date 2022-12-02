shell_objects = shell_backbone.o \
				shell_main.o \
				shell_command_parser.o \
				shell_prompt.o \
				shell_builtin.o \
				shell_overkill.o \
				shell_kjob.o \
				shell_jobs.o \
				shell_echo.o \
				shell_no_fork_builtin.o \
				shell_fg_bg.o \
				shell_cd.o \
				shell_quit.o \
				shell_pwd.o \
				shell_ls.o \
				shell_env.o \
				shell_history.o \
				shell_cronjob.o

executables = 	shell \
				pinfo \
				nightswatch

all : $(executables)

shell: $(shell_objects)
	gcc -o shell $(shell_objects)

shell_backbone.o: shell_backbone.c shell_struct.h shell_builtin.h
	gcc -g -c shell_backbone.c

shell_main.o : shell_main.c
	gcc -g -c shell_main.c

shell_command_parser.o : shell_command_parser.c shell_struct.h
	gcc -g -c shell_command_parser.c

shell_builtin.o : shell_builtin.c shell_builtin.h
	gcc -g -c shell_builtin.c

shell_overkill.o : shell_overkill.c shell_struct.h
	gcc -g -c shell_overkill.c

shell_kjob.o : shell_kjob.c shell_struct.h
	gcc -g -c shell_kjob.c

shell_jobs.o : shell_jobs.c shell_struct.h
	gcc -g -c shell_jobs.c

shell_fg_bg.o : shell_fg_bg.c shell_struct.h
	gcc -g -c shell_fg_bg.c

shell_prompt.o : shell_prompt.c
	gcc -g -c shell_prompt.c	

shell_cd.o : shell_cd.c
	gcc -g -c shell_cd.c	

shell_no_fork_builtin.o : shell_no_fork_builtin.c shell_no_fork_builtin.h
	gcc -g -c shell_no_fork_builtin.c

shell_quit.o : shell_quit.c
	gcc -g -c shell_quit.c

shell_echo.o : shell_echo.c
	gcc -g -c shell_echo.c

shell_pwd.o : shell_pwd.c
	gcc -g -c shell_pwd.c

shell_ls.o : shell_ls.c
	gcc -g -c shell_ls.c

shell_env.o : shell_env.c
	gcc -g -c shell_env.c

shell_history.o : shell_history.c
	gcc -g -c shell_history.c

shell_cronjob.o : shell_cronjob.c
	gcc -g -c shell_cronjob.c

pinfo : pinfo.c
	gcc -o pinfo pinfo.c

nightswatch : nightswatch.c
	gcc -o nightswatch nightswatch.c

clean:
	rm $(executables) $(shell_objects)
