//============================================================
//Date:		April 5,2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		token.h
//============================================================

#ifndef TOKEN_H
#define TOKEN_H

enum tokenID{
	ID_tk,
	NUM_tk,
	KW_tk,
	OP_tk,
	EOF_tk,
	ERROR_tk
};

typedef struct{

	enum tokenID id;
	char string[24];
	int line;
}Token;

void printToken(Token *);
char *getTokenId(int);
void driveScanner();
int getChar();
void filter();
Token *fsa_driver();
void append(char *, char);
void printError(int, int);
#endif

