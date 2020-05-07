//===================================================
//Date:		March 8, 2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		functions.c
//===================================================

#include "filter.h"

void stdinFp(FILE *stream)
{
	if((fseek(stream, 0, SEEK_END), ftell(stream)) > 0)
	{
		rewind(stream);
	}
	else
	{
		perror("ERROR: stdin: functions\n");
		exit(1);
	}
}

void returnFp(char *argv)
{
	FILE *stream;

	stream = fopen(argv, "r");

	if(stream == NULL)
	{
		perror("ERROR: stream: functions\n");
		exit(1);
	}
	else if(fseek(stream, 0, SEEK_END), ftell(stream) == 0)
	{
		fprintf(stderr, "File is empty!\n");
		exit(EXIT_FAILURE);
	}
}

char * handleSingleNode(char *argv)
{
	FILE *fp;

	char *tempFile = "test.txt";

	fp = fopen(tempFile, "w");

	if(fp == NULL)
	{
		perror("ERROR: fp: functions\n");
		exit(1);
	}
	else
	{
		fprintf(fp, "%s", argv);
	}

	fclose(fp);

	return tempFile;
}

char *handleNodeString(int argc, char *argv[])
{
	FILE *fp;

	char *tempFile = "test.txt";

	fp = fopen(tempFile, "w");

	if(fp == NULL)
	{
		perror("ERROR: fp: functions\n");
		exit(1);
	}
	else
	{
		int i;
		for(i = 1; i < argc; ++i)
		{
			fprintf(fp, "%s ", argv[i]);
		}
	}

	fclose(fp);

	return tempFile;
}
