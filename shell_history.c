#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_OUTPUT 10
#define MAX_STORE 20
#define CL 256  // command length


typedef struct Node
{
	char command[CL];
	struct Node *next;
} Node;


void historyInit();
void historyStore(char *command);
void history(char **argv);
void historyUpArrow(char *command);


static Node *head = NULL;
static char fileLoc[256];


/*
	Initializes history. Use after setting home_dir environment variable.
*/
void historyInit()
{
	char *shellHome;
	if ( (shellHome = getenv("home_dir")) == NULL )
	{
		fprintf(stderr, "history: home_dir retrival error.\n");
		strcpy(fileLoc, "history.txt");
	}
	else
	{
		strcpy(fileLoc, shellHome);
		strncat(fileLoc, "/history.txt", strlen("/history.txt"));
	}
	
	FILE *fd = fopen(fileLoc, "a+");

	char command[CL];
	while ( fgets(command, CL, fd) )
	{
		Node *node = malloc( sizeof(Node) );
		strncpy(node->command, command, CL);
		node->next = NULL;

		if (head == NULL)
			head = node;
		else
		{
			node->next = head;
			head = node;
		}
	}

	fclose(fd);
}


void historyStore(char *command)
{
	Node *node = malloc( sizeof(Node) );
	strncpy(node->command, command, CL);
	node->next = NULL;

	if (head == NULL)
		head = node;
	else
	{
		node->next = head;
		head = node;
	}

	FILE *fd = fopen(fileLoc, "w");

	int cnt = 0;
	Node *cur = head;
	Node *prev = NULL;
	while (cur && cnt < MAX_STORE)
	{
		fputs(cur->command, fd);
		prev = cur;
		cur = cur->next;
		cnt++;
	}

	if(cnt == MAX_STORE && cur != NULL)
	{
		prev->next = NULL;
		free(cur);
	}

	fclose(fd);
}


void history(char **argv)
{
	int num;
	
	if (argv[1] == NULL)
		num = MAX_OUTPUT;
	
	else if ( atoi(argv[1]) <= MAX_OUTPUT  )
		num = atoi(argv[1]);
	
	else
	{
		fprintf(stderr, "history: Error in arguments\n");
		return;
	}

	Node *cur = head;
	int i = 0;

	while(cur && i < num)
	{
		printf("%s", cur->command);
		cur = cur->next;
		i++;
	}
}


/*
	Up arrow key: ASCII = 27 91 65 => strlen(UP) = 3.
	Function takes in up arrow keys, 
	changes string pointed by command pointer to the no.of up arrow key previous command. 
*/
void historyUpArrow(char *command)
{
	if (head == NULL)
	{
		printf("history: Error, not enough history.\n");
		command[0] = '\n';
		command[1] = '\0';
		return;	
	}

	int up = (strlen(command) - 1) / 3 - 1;
	
	if (up >= 10)
	{
		printf("history: Error, max up arrow 10.\n");
		command[0] = '\n';
		command[1] = '\0';
		return;		
	}

	Node *cur = head;

	for (int i=0; i<up; i++)
	{
		if (cur->next == NULL)
		{
			printf("history: Error, not enough history.\n");
			command[0] = '\n';
			command[1] = '\0';
			return;
		}

		cur = cur->next;
	}

	strcpy(command, cur->command);
}

