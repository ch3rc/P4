//======================================================
//Date:		Apr 24,2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P2
//File:		node.h
//======================================================

#ifndef NODE_H
#define NODE_H

struct Node{

	struct Node* child;
	char* token;
	int size;
	int id;
};

struct Node *newNode(char*);
void getToken(struct Node*, struct Node*);
void preOrder(struct Node*, int);
void SSTV(struct Node *);
void staticSemantics(struct Node *);

//test functions
void generate(struct Node *, FILE *);
void aVars(struct Node *, FILE *);
void aBlock(struct Node *, FILE *);
void aExpr(struct Node *, FILE *);
void aN(struct Node *, FILE *);
void aA(struct Node *, FILE *);
void aM(struct Node *, FILE *);
void aR(struct Node *, FILE *);
void aStats(struct Node *, FILE *);
void amStat(struct Node *, FILE *);
void aStat(struct Node *, FILE *);
void aIn(struct Node *, FILE *);
void aOut(struct Node *, FILE *);
void aAssign(struct Node *, FILE *);
void aIf(struct Node *, FILE *);
void aRO(struct Node *, FILE *);

#endif
