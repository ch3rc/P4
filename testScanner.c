//===============================================
//Date:		April 5,2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		testScanner.c
//===============================================

#include "filter.h"
#include "table.h"
#include "token.h"

FILE *file;

void driveScanner()
{
	/*get one token at a time and print it*/
	while(1)
	{
		Token *token = NULL;
		token = (Token *)malloc(sizeof(Token));
		token = fsa_driver();
		printToken(token);	
		if(token->id == EOF_tk)
		{
			break;
		}
		if(token->id == ERROR_tk)
		{
			//printf("made it into error break\n");
			break;
		}
		free(token);
	}

	fclose(file);
	return 0;
	
}

