#include <stdio.h>

void echo (char **argv)
{
	if (argv[1] == NULL)
		return;

	char *str = argv[1];

	while (*str == ' ' || *str == '\t')
		str++;

	printf("%s\n", str);
}

