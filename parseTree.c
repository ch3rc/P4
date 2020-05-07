//==========================================================
//Date:		Apr 24,2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P2
//File:		parseTree.c
//==========================================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "token.h"
#include "table.h"
#include "node.h"

int count = 0;
int idCount = 0;
int inCount = 0;
int aCount = 0;
int labCount = 0;
int goCount = 0;
int rCount = 0;

char *declared[50];

//added memory to word tokens and things started working
struct Node *newNode(char* token)
{
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	temp->token = (char*)malloc(sizeof(token) + 1);
	strcpy(temp->token, token);
	temp->size = 0;
	temp->child = NULL;
	return temp;
}

//increase the size of the parent node for each additional child that attaches
//to it
void getNode(struct Node* parent, struct Node* child)
{
	struct Node *temp = newNode("");
	temp->child = (struct Node*)malloc(sizeof(struct Node));
	parent->child = (struct Node*)realloc(parent->child, sizeof(struct Node) * (parent->size + 1));
	temp->child = child;
	parent->child[parent->size] = *(temp->child);
	parent->size++;
}

//print out all parents and children
void preOrder(struct Node *root, int level)
{
	if(root->token != NULL)
	{
		printf("%*c%s\n", level * 2, ' ', root->token);
		
		int i = 0;
		while(i < root->size)
		{
			preOrder(&root->child[i], level + 1);
		}
	}

}

void SSTV(struct Node *root)
{
	staticSemantics(root);

	if(root->token != NULL)
	{
		int i;
		for(i = 0; i < root->size; i++)
		{
			SSTV(&root->child[i]);
		}
	}
}

void staticSemantics(struct Node *root)
{
	int i, j;
	
	//check in scope of vars to make sure that the user is not declaring
	//the same variable twice
	if(strcmp(root->token, "<vars>") == 0)
	{
		for(i = 0; i < root->size; i++)
		{
			if(root->child[i].id == ID_tk && strcmp(root->child[i].token,"<vars>") != 0)
			{
				declared[count] = malloc(sizeof(root->child[i].token)*sizeof(char));
				strcpy(declared[count], root->child[i].token);
				count++;
			}
		}
		for(j = 0; j < count; j++)
		{
			for(i = 0; i < root->size; i++)
			{
				if(strcmp(declared[j], root->child[i].token) == 0)
				{	
					idCount++;
				}
				if(idCount > count)
				{
					printf("<vars> Declaring variable %s more than once\n", declared[j]);
					exit(1);
				}
			}
		}
		idCount = 0;		
	}

	//check for non declared variables in <in> production
	else if(strcmp(root->token, "<in>") == 0)
	{
		for(i = 0; i < count; i++)
		{
			for(j = 0; j < root->size; j++)
			{
				if(root->child[j].id == ID_tk && strcmp(root->child[j].token, "<in>") != 0)
				{
					if(strcmp(declared[i], root->child[j].token) == 0)
					{
						inCount++;
					}
				}
			}
		}

		if(inCount == 0)
		{
			for(i = 0; i < root->size; i++)
			{
				if(root->child[i].id == ID_tk && strcmp(root->child[i].token, "<in>") != 0)
				{
					printf("<in> Variable %s not declared at the beginning of the program\n", root->child[i].token);
					exit(1);
				}
			}
		}
		inCount = 0;

	}

	//check for non declared variables in <assign> production
	else if(strcmp(root->token, "<assign>") == 0)
	{
		for(i = 0; i < count; i++)
		{
			for(j = 0; j < root->size; j++)
			{
				if(root->child[j].id == ID_tk && strcmp(root->child[j].token, "<assign>") != 0)
				{
					if(strcmp(declared[i], root->child[j].token) == 0)
					{
						aCount++;
					}
				}
			}
		}

		if(aCount == 0)
		{
			for(i = 0; i < root->size; i++)
			{
				if(root->child[i].id == ID_tk && strcmp(root->child[i].token, "<assign>") != 0)
				{
					printf("<assign> Variable %s not declared at the beginning of the program\n", root->child[i].token);
					exit(1);
				}
			}
		}
		aCount = 0;
	}

	//check for non declared variables in <label> production
	else if(strcmp(root->token, "<label>") == 0)
	{
		for(i = 0; i < count; i++)
		{
			for(j = 0; j < root->size; j++)
			{
				if(root->child[j].id == ID_tk && strcmp(root->child[j].token, "<label>") != 0)
				{
					if(strcmp(declared[i], root->child[j].token) == 0)
					{
						labCount++;
					}
				}
			}
		}

		if(labCount == 0)
		{
			for(i = 0; i < root->size; i++)
			{
				if(root->child[i].id == ID_tk && strcmp(root->child[i].token, "<label>") != 0)
				{
					printf("<label> Variable %s not declared at the beginning of the program\n", root->child[i].token);
					exit(1);
				}
			}
		}
		labCount = 0;
	}

	//check for non declared variables in <goto> production
	else if(strcmp(root->token, "<goto>") == 0)
	{
		for(i = 0; i < count; i++)
		{
			for(j = 0; j < root->size; j++)
			{
				if(root->child[j].id == ID_tk && strcmp(root->child[j].token, "<goto>") != 0)
				{
					if(strcmp(declared[i], root->child[j].token) == 0)
					{
						goCount++;
					}
				}
			}
		}

		if(goCount == 0)
		{
			for(i = 0; i < root->size; i++)
			{
				if(root->child[i].id == ID_tk && strcmp(root->child[i].token, "<goto>") != 0)
				{
					printf("<goto> Variable %s not declared at the beginning of the program\n", root->child[i].token);
					exit(1);
				}
			}
		}
		goCount = 0;
	}

	//check for non declared variables in <R> production
	else if(strcmp(root->token, "<R>") == 0)
	{
		for(i = 0; i < count; i++)
		{
			for(j = 0; j < root->size; j++)
			{
				if(root->child[j].id == ID_tk && strcmp(root->child[j].token, "<R>") != 0)
				{
					if(strcmp(declared[i], root->child[j].token) == 0)
					{
						rCount++;
					}
				}
			}
		}

		if(rCount == 0)
		{
			for(i = 0; i < root->size; i++)
			{
				if(root->child[i].id == ID_tk && strcmp(root->child[i].token, "<R>") != 0)
				{
					printf("<R> Variable %s not declared at the beginning of the program\n", root->child[i].token);
					exit(1);
				}
			}
		}
		rCount = 0;
	}
}
