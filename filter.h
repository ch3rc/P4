//=========================================================
//Date:		March 8, 2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		filter.h
//=========================================================

#ifndef FILTER_H
#define FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *checkFile(int argc, char *argv[]);

void stdinFP(FILE *stream);

void returnFp(char *argv);

char *handleSingleNode(char *argv);

char *handleNodeString(int argc, char *argv[]);

#endif
