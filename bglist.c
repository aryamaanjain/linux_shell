#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bglist.h"


typedef struct Node
{
	pid_t pid;
	char pname[128];
	struct Node *next;
} Node;


static Node *head = NULL;


void bgInsert(pid_t pid, char pname[])
{
	Node *node = malloc(sizeof(Node));
	node->pid = pid;
	strncpy(node->pname, pname, 128);
	node->next = NULL;

	if(head == NULL)
	{
		head = node;
		return;
	}

	Node *cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = node;
}


void bgFindDelete(pid_t pid)
{
	if (head == NULL)
		return;

	if (head->pid == pid)
	{
		Node *temp = head;
		head = head->next;
		printf("%s with ", temp->pname);
		free(temp);
		return;
	}

	Node *cur = head;
	while (cur->next != NULL)
	{
		if (cur->next->pid == pid)
		{
			Node *temp = cur->next;
			cur->next = temp->next;
			printf("%s with ", temp->pname);
			free (temp);
			return;
		}
		cur = cur->next;
	}
}

