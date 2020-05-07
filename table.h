//==============================================================
//Date:		April 5,2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		table.h
//==============================================================

#ifndef TABLE_H
#define TABLE_H
#include "token.h"

extern int lineCount;
extern FILE *file;

enum State{
	START_st,   //0
	ID_st,      //1
	NUM_st,     //2
	COLON_st,   //3
	EQUAL_st,   //4
	PLUS_st,    //5
	MINUS_st,   //6
	TIMES_st,   //7
	DIV_st,     //8
	MOD_st,     //9
	PER_st,     //10
	LF_PAR_st,  //11
	RT_PAR_st,  //12
	COMMA_st,   //13
	LT_BRC_st,  //14
	RT_BRC_st,  //15
	SEMI_st,    //16
	LT_BRK_st,  //17
	RT_BRK_st,  //18
	LESS_st,    //19
	GREATER_st, //20
	EOF_st,     //21
	ERROR_st,   //22
};

int tableLookup(enum State, char );
int getSpecial(char);
#endif
