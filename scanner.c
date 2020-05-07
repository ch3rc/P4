//====================================================================
//Date:		March 4,2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		scanner.c
//====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "token.h"
#include "table.h"

int lineCount = 0;
int nextChar;
enum State state;
int nextState;
FILE *file;

/*keywords to check tokens against*/
const char *keywords[] = { 
	"label",
	"goto",
	"void",
	"declare",
	"return",
	"loop",
	"in",
	"out",
	"program",
	"iffy",
	"then",
	"assign",
	"data", 
};
   
/*lenght of keyword array*/
#define len (sizeof(keywords) / sizeof(const char *))

Token *fsa_driver()
{
	nextChar = getChar();
	state = START_st;
	Token *tid = NULL;
	tid = (Token *)malloc(sizeof(Token));
	char str[100] = "";
	while(state < 1000)
	{

		/*get next state from FSA TABLE*/
		nextState = tableLookup(state, nextChar);
		
		if(nextState < -1)
		{
			/*print out error message and end program*/
			printError(nextState, lineCount);
			fclose(file);
			exit(1);
		}
		//else if(feof(file))
		else if(nextState == -1)
		{
			/*reached end of file*/
			tid->id = EOF_tk;
			strcpy(tid->string, "EOF");
			tid->line = lineCount;
			return tid;
		}
		else if(nextState >= 1000)
		{
			/*if equal to final state check if keyword*/
			if(nextState == 1000)
			{
				int i;
				for(i = 0; i < len; ++i)
				{
					/*search through keywords*/
					if(strstr(str, keywords[i]))
					{
						tid->id = KW_tk;
						strcpy(tid->string, str);
						tid->line = lineCount;
						return tid;
					}
				}
				/*if not in keyword list its an identifier*/
				//fprintf(stderr, "ID nextState = %d, state = %d\n", nextState, state);
				ungetc(nextChar, file);
				tid->id = ID_tk;
				strcpy(tid->string, str);
				tid->line = lineCount;
				return tid;
				
			}
			else
			{
				/*if greater than 1001 its a special character, 1001 is a digit*/
				if(nextState > 1001)
				{	
					//fprintf(stderr, "OP nextState = %d, state = %d\n", nextState, state);
					ungetc(nextChar, file);
					tid->id = OP_tk;
					strcpy(tid->string, str);
					tid->line = lineCount;
					return tid;
				}
				else
				{
					ungetc(nextChar, file);
					tid->id = NUM_tk;
					strcpy(tid->string, str);
					tid->line = lineCount;
					return tid;
				}
			}
		}
		else
		{
			/*get next character if not final state*/
			state = nextState;
			append(str, nextChar);
			nextChar = getChar();
			filter();
		}

	}
}

/*get next charachter*/
int getChar()
{
	int c;
	c = fgetc(file);
	//fprintf(stderr, "nextChar = %c\n", (char)c);
	return (char)c;
}

/*filter out comments and count new lines*/
void filter()
{
	int truth;

	if(nextChar == '\n')
	{	
		while(nextChar == '\n')
		{
			lineCount++;
			nextChar = getChar();
		}
	}
			
	if(nextChar == '#')
		truth = 1;

	if(truth == 1)
	{
		
		
		do
		{
			if(nextChar == '\n')
				lineCount++;

			nextChar = getChar();

			if(nextChar < 0)
			{
				fprintf(stderr, "Unclosed Comment\n");
				exit(1);
			}
			truth = 0;
		
		}while(nextChar != '#');
		nextChar = getChar();
	}
	
}

/*build token string*/
void append(char *s, char c)
{
	if(c != ' ' && c != '\n')
	{	
		int leng = strlen(s);
		s[leng] = c;
		s[leng + 1] = '\0';
	}
}

/*naive error check for now until further instruction on how language works*/
void printError(int e, int line)
{

	if(e == -2)
	{
		printf("SCANNER ERROR: Unknown character on line %d\n", line);
	}
	else if(e == -3)
	{
		printf("SCANNER ERROR: Equal sign before colon on line %d\n", line);
	}
}
