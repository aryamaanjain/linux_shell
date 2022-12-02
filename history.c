#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_OUTPUT 10
#define MAX_STORE 20
#define CL 128  // command length


typedef struct Node
{
	char command[CL];
	struct Node *next;
} Node;


void historyInit();
void historyStore(char *command);
void history(int argc, char *argv[]);


static Node *head = NULL;
static char fileLoc[256];


void historyInit()
{
	char *shellHome;
	if ( (shellHome = getenv("shellHome")) == NULL )
	{
		fprintf(stderr, "history shellHome retrival error\n");
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


void history(int argc, char *argv[])
{
	int num;
	
	if (argc == 1)
		num = MAX_OUTPUT;
	
	else if ( atoi(argv[1]) <= MAX_OUTPUT  )
		num = atoi(argv[1]);
	
	else
	{
		fprintf(stderr, "history: error in arguments\n");
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

