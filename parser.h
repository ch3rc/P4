//================================================================
//Date:		Apr 21,2020
//Author:	Cody Hawkins
//Class: 	CS4280
//Project:	P2
//File:		Parser.h
//================================================================
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "table.h"

struct Node *parser();
struct Node *block();
struct Node *vars();
struct Node *expr();
struct Node *N();
struct Node *A();
struct Node *M();
struct Node *R();
struct Node *stats();
struct Node *mStat();
struct Node *stat();
struct Node *in();
struct Node *out();
struct Node *iffy();
struct Node *loop();
struct Node *assign();
struct Node *label();
struct Node *Goto();
struct Node *RO();

#endif
