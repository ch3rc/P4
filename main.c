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

FILE *file;

int main(int argc, char *argv[])
{
	//check files
	file = checkFile(argc, argv);

	if(file == NULL)
	{
		perror("ERROR: file: main\n");
		exit(1);
	}
	
	/*once file has been checked send to driver*/
	//driveScanner();	
	struct Node *root = parser();
	SSTV(root);
	//preOrder(root, 0);
	return 0;
}	
