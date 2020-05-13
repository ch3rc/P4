//===============================================================
//Date:		March 4, 2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P1
//File:		main.c
//===============================================================

#include "filter.h"
#include "table.h"
#include "node.h"
#include "parser.h"

int kb_flag;
int file_flag;
int std_in_flag;
FILE *file;


int main(int argc, char *argv[])
{
	char *fileName;
	FILE *fp;
	//check files
	file = checkFile(argc, argv);

	if(file == NULL)
	{
		perror("ERROR: file: main\n");
		exit(1);
	}
	
	/*once file has been checked send to driver*/
	//driveScanner();
	//preOrder(root, 0);
	
	struct Node *root = parser();
	SSTV(root);

	if(kb_flag)
	{
		fp = fopen("kb.asm", "w");
		fprintf(stderr, "Assebmly language in %s\n", fileName);
		if(fp == NULL)
		{
			perror("FILE");
		}
		generate(root, fp);
	}
	else if(file_flag)
	{
		fileName = fileBasename(argv[1]);
		fprintf(stderr, "Assembly language in %s\n", fileName);
		fp = fopen(fileName, "w");
		if(fp == NULL)
		{
			perror("FILE");
		}
		generate(root, fp);
	}
	else if(std_in_flag)
	{
		fprintf(stderr, "Assembly language in file.asm\n");
		fp = fopen("file.asm", "w");
		if(fp == NULL)
		{
			perror("FILE");
		}
		generate(root, fp);
	}
	return 0;
}	
