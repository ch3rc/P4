//===============================================
//Date:		April 5,2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		table.c
//===============================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "table.h"

int lineCount;
FILE *file;

/*current FSA TABLE will add more errors as we get more on how language works*/
int fsa_table[21][23] = {
/*W     C    D      :     =       +     -     *    /      %     .    (    )       ,    {     }     ;     [      ]    <    >    EOF   ERROR*/
{ 0,    1,    2,    3,    4,      5,    6,   7,    8,     9,   10,   11,  12,    13,   14,   15,   16,   17,   18,   19,   20,    -1,   -2},
{1000,  1,    1,   1000,  1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
{1001, 1001,  2,   1001,  1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001},
{1002, 1002, 1002,    3,    4,  1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002},
{1003, 1003, 1003,   -3,    4,  1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003},
{1004, 1004, 1004, 1004,  1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004},
{1005, 1005, 1005, 1005,  1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005},
{1006, 1006, 1006, 1006,  1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006},
{1007, 1007, 1007, 1007,  1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007},
{1008, 1008, 1008, 1008,  1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008},
{1009, 1009, 1009, 1009,  1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009},
{1010, 1010, 1010, 1010,  1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010},
{1011, 1011, 1011, 1011,  1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011},
{1012, 1012, 1012, 1012,  1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012},
{1013, 1013, 1013, 1013,  1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013},
{1014, 1014, 1014, 1014,  1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014},
{1015, 1015, 1015, 1015,  1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015},
{1016, 1016, 1016, 1016,  1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016},
{1017, 1017, 1017, 1017,  1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017},
{1018, 1018, 1018, 1018,  1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018},
{1019, 1019, 1019, 1019,  1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019}
};

/*check through fsa table and changing state*/
int tableLookup(enum State state, char c)
{
	//printf("made it into the tableLookup\n");
	int current;
	
	if(c == '\n')
	{
		current = 0;
		//lineCount++;
	}
	else if(c == ' ' || c == '\t')
	{
		current = 0;
	}
	else if(isalpha(c) != 0)
	{
		current = 1;
	}

	else if(isdigit(c) != 0)
	{
		current = 2;
	}
	else if(ispunct(c) != 0)
	{
		current = getSpecial(c);
	}
	else if(c == NULL)
	{
		current = 0;
	}
	else if(feof(file))
	{
		current = 21;
	}
	else
	{
		current = 22;
	}
	
	//printf("c = %c, state = %d, current = %d, value = %d\n", c, state, current, fsa_table[state][current]);
	return fsa_table[state][current];
}

/*changing tokenID to string literal*/
char *getTokenID(int id)
{
	//printf("made it to the id switch\n");
	char *temp;

	switch(id)
	{
		case ID_tk: 
			temp = "Identifier";
			return temp;
		case NUM_tk:
			temp = "Number"; 
			return temp;
		case KW_tk:
			temp = "Keyword"; 
			return temp;
		case OP_tk:
			temp = "Operator";
			return temp;
		case EOF_tk:
			temp = "EOF"; 
			return temp;
		case ERROR_tk:
			temp = "ERROR"; 
			return temp;
		default:
			temp = "INVALID"; 
			return temp;
	}
}

/*print out token value*/
void printToken(Token *ptr)
{
	//printf("made it to the printToken\n");
	char *tokID;
	//printf("before token id\n");
	tokID = getTokenID(ptr->id);
	//printf("got past token id");
	printf("\n<%s %s %d>\n", tokID, ptr->string, ptr->line);
}

/*search through list of special characters and return state in fsa table*/
int getSpecial(char c)
{
	switch(c)
	{
		case ':':
			return 3;
		case '=':
			return 4;
		case '+':
			return 5;
		case '-':
			return 6;
		case '*':
			return 7;
		case '/':
			return 8;
		case '%':
			return 9;
		case '.':
			return 10;
		case '(':
			return 11;
		case ')':
			return 12;
		case ',':
			return 13;
		case '{':
			return 14;
		case '}':
			return 15;
		case ';':
			return 16;
		case '[':
			return 17;
		case ']':
			return 18;
		case '<':
			return 19;
		case '>':
			return 20;
		default:
			//error currently
			return 22;
	}
}

