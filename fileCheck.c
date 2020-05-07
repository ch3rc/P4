//===============================================================
//Date:		March 4, 2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		fileCheck.c
//===============================================================

#include "filter.h"

int fileno(FILE *stream);

FILE *checkFile(int argc, char *argv[])
{
	FILE *fp;
	char *file;

	if(argc == 1)
	{
		if(isatty(fileno(stdin)) == 0)
		{
			stdinFp(stdin);

			fp = stdin;

			//basename = stdinBasename();
		}
		else
		{
			printf("please enter a file, redirect a file or type one or more words\n");
			exit(1);
		}
	}
	else if(argc == 2)
	{
		if(strstr(argv[1], "."))
		{
				returnFp(argv[1]);

				fp = fopen(argv[1], "r");

				//basename = fileBasename(argv[1]);
		}
		else if(strstr(argv[1], "_") && !strstr(argv[1], "."))
		{
			fp = fopen(argv[1], "r");
			if(fp == NULL)
			{
				perror("ERROR: fp: functions\n");
				exit(1);
			}
		}
		else
		{
			file = handleSingleNode(argv[1]);

			fp = fopen(file, "r");

			//basename = "output";
		}

	}
	else
	{
		//basename = "ouput";

		file = handleNodeString(argc, argv);

		fp = fopen(file, "r");

	}

	if(fp == NULL)
	{
		perror("ERROR: fp: filter\n");
		exit(EXIT_FAILURE);
	}

	return fp;
}
