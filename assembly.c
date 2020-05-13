//========================================================
//Date: 	May 8, 2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P4 (assembly generator)
//File:		Assembly.c
//========================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "token.h"
#include "table.h"
#include "node.h"

char *idArray[50];
int iCount = 0;
int fromAss = 0;
int mult = 0;
int add = 0;
int sub = 0;
int ify = 0;

static int LabelCntr = 0;
static int VarCntr = 0;
typedef enum {VAR, LABEL} nameType;
static char Name[20];

static char *newName(nameType what)
{
	if(what == VAR)
		sprintf(Name, "T%d", VarCntr++);
	else
		sprintf(Name, "L%d", LabelCntr++);
	return Name;
}

void generate(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in generate\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<vars>") == 0)
		{
			aVars(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<block>") == 0)
		{
			aBlock(&root->child[i], file);
		}
	}
	fprintf(file, "STOP\n");
	for(i = 0; i < iCount; i++)
	{
		if(strstr(idArray[i], "L"))
		{
			continue;
		}
		else
		{
			fprintf(file, "%s 0\n", idArray[i]);
		}
	}
	fclose(file);

}

void aVars(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in vars\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(root->child[i].id == NUM_tk)
		{
			fprintf(file, "LOAD %s\n", root->child[i].token);
			fprintf(file, "STORE %s\n", idArray[iCount - 1]);
		}
		else if(root->child[i].id == ID_tk && strcmp(root->child[i].token, "<vars>") != 0)
		{
			idArray[iCount] = malloc(sizeof(root->child[i].token) * sizeof(char));
			strcpy(idArray[iCount], root->child[i].token);
			iCount++;
		}
		else if(strcmp(root->child[i].token, "<vars>") == 0)
		{
			aVars(&root->child[i], file);
		}
	}
}

void aBlock(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in block\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<vars>") == 0)
		{
			aVars(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<stats>") == 0)
		{
			aStats(&root->child[i], file);
		}
	}
}

void aExpr(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in expr\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<N>") == 0)
		{
			aN(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "-") == 0)
		{
			sub = 1;
			aExpr(&root->child[i + 1], file);
			fprintf(file, "LOAD %s\n", idArray[iCount - 1]);
			fprintf(file, "SUB %s\n", idArray[iCount - 2]);
			fprintf(file, "STORE %s\n", idArray[iCount - 1]);
		}
	}
}

void aN(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in N\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<A>") == 0)
		{
			aA(&root->child[i], file);
			if(strcmp(root->child[i].token, "/") == 0)
			{
				aN(&root->child[i], file);
			}
			else if(strcmp(root->child[i].token, "*") == 0)
			{
				aN(&root->child[i], file);
			}
		}
	}
}

void aA(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in A\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<M>") == 0)
		{
			aM(&root->child[i], file);
		}
		else if(strcmp(root->child[i].token, "+") == 0)
		{
			add = 1;
			aA(&root->child[i + 1], file);
		}
	}
}

void aM(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in M\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "*") == 0 && strcmp(root->child[i + 1].token, "<M>") == 0)
		{
			mult = 1;
			aM(&root->child[i + 1], file);
			fprintf(file, "MULT -1\n");
		}
		else
		{
			aR(&root->child[i], file);
		}
	}
}

void aR(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in R\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<expr>") == 0)
		{
			aExpr(&root->child[i], file);
		}
		else if(root->child[i].id == ID_tk && strcmp(root->child[i].token, "<R>") != 0)
		{
			fprintf(file, "LOAD %s\n", root->child[i].token);
		}
		else if(root->child[i].id == NUM_tk)
		{
			if(fromAss)
			{
				fprintf(file, "LOAD %s\n", root->child[i].token);
			}
			else if(mult && !ify)
			{
				fprintf(file, "LOAD %s\n", root->child[i].token);
				idArray[iCount] = malloc(sizeof(root->child[i].token) * sizeof(char));
				strcpy(idArray[iCount], newName(0));
				iCount++;
			}
			else if(add || sub)
			{
				fprintf(file, "LOAD %s\n", root->child[i].token);
				idArray[iCount] = malloc(sizeof(root->child[i].token) * sizeof(char));
				strcpy(idArray[iCount], newName(0));
				fprintf(file, "STORE %s\n", idArray[iCount]);
				iCount++;
			}
			else if(!fromAss || !mult)
			{
				fprintf(file, "LOAD %s\n", root->child[i].token);
				idArray[iCount] = malloc(sizeof(root->child[i].token) * sizeof(char));
				strcpy(idArray[iCount], newName(0));
				fprintf(file, "STORE %s\n", idArray[iCount]);
				iCount++;
			}
		}
	}
}


void aStats(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in stats\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<stat>") == 0)
		{
			aStat(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<mStat>") == 0)
		{
			amStat(&root->child[i], file);
		}
	}
}

void amStat(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in mStat\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<stat>") == 0)
		{
			aStat(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<mStat>") == 0)
		{
			amStat(&root->child[i], file);
		}
	}
}

void aStat(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in stat\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<in>") == 0)
		{
			aIn(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<out>") == 0)
		{
			aOut(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<block>") == 0)
		{
			aBlock(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<if>") == 0)
		{
			aIf(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<loop>") == 0)
		{
			//aLoop(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<assign>") == 0)
		{
			aAssign(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<goto>") == 0)
		{
			//aGoto(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<label>") == 0)
		{
			//aLabel(&root->child[i], file);
		}
	}
}

void aIn(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in IN\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(root->child[i].id == ID_tk && strcmp(root->child[i].token, "<in>") != 0)
		{
			fprintf(file, "READ %s\n", root->child[i].token);
		}
	}
}

void aOut(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in OUT\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<expr>") == 0)
		{
			aExpr(&root->child[i], file);
			if(mult)
			{
				fprintf(file, "STORE %s\n", idArray[iCount - 1]);
				fprintf(file, "WRITE %s\n", idArray[iCount - 1]);
			}
			else if(add && sub)
			{
				fprintf(file, "LOAD %s\n", idArray[0]);
				fprintf(file, "ADD %s\n", idArray[iCount - 1]);
				fprintf(file, "STORE %s\n", idArray[0]);
				fprintf(file, "WRITE %s\n", idArray[0]);
			}
			else if(idArray[iCount] == NULL)
			{
				fprintf(file, "WRITE %s\n", idArray[iCount - 1]); // need to figure out how to return variable
			}
			else
			{
				fprintf(file, "WRITE %s\n", idArray[iCount]);
			}
		}
	}
}

void aAssign(struct Node *root, FILE *file)
{
	//fprintf(stderr, "in assign\n");
	int i;
	char word[20];
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<expr>") == 0)
		{
			fromAss = 1;
			aExpr(&root->child[i], file);
			fprintf(file, "STORE %s\n", word);
		}
		if(root->child[i].id == ID_tk)
		{
			sprintf(word, "%s", root->child[i].token);
		}
	}
}

void aIf(struct Node *root, FILE *file)
{
	ify = 1;
	//fprintf(stderr, "in IF\n");
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<expr>") == 0)
		{
			aExpr(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<RO>") == 0 && strcmp(root->child[i + 1].token, "<expr>") == 0)
		{
			aExpr(&root->child[i + 1], file);
			aRO(&root->child[i], file);
		}
		if(strcmp(root->child[i].token, "<stat>") == 0)
		{
			aStat(&root->child[i], file);
		}
	}
}

void aRO(struct Node *root, FILE *file)
{
	int i;
	for(i = 0; i < root->size; i++)
	{
		if(strcmp(root->child[i].token, "<") == 0)
		{
			fprintf(file, "LOAD %s\n", idArray[iCount - 1]);
			fprintf(file, "SUB %s\n", idArray[0]);
			idArray[iCount] = malloc(sizeof(root->child[i].token) * sizeof(char));
			strcpy(idArray[iCount], newName(1));
			iCount++;
			fprintf(file, "BRPOS %s\n", idArray[iCount - 1]);
			fprintf(file, "%s: NOOP\n", idArray[iCount - 1]);
		}
		else if(strcmp(root->child[i].token, ">") == 0)
		{
			fprintf(file, "LOAD %s\n", idArray[0]);
			fprintf(file, "SUB %s\n", idArray[iCount - 1]);
			idArray[iCount] = malloc(sizeof(root->child[i].token) * sizeof(char));
			strcpy(idArray[iCount], newName(1));
			iCount++;
			fprintf(file, "BRPOS %s\n", idArray[iCount - 1]);
			fprintf(file, "%s: NOOP\n", idArray[iCount - 1]);
		}
	}
}
			

