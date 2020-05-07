//============================================================
//Date:		Apr 21,2020
//Author:	Cody Hawkins
//Class:	CS4280
//Project:	P2
//File:		parser.c
//============================================================

#include "parser.h"
#include "node.h"

Token *tk = NULL;

struct Node *parser()
{
	tk = fsa_driver();
	struct Node *programProd = newNode("<program>");

	struct Node *varsProd = vars();
	getNode(programProd, varsProd);

	struct Node *blockProd = block();
	getNode(programProd, blockProd);
	
	return programProd;
	if(tk->id == EOF_tk)
	{
		return programProd;
	}
	else
	{
		printf("Tk received, encountered EOF error\n");
		free(tk);
		exit(1);
	}
}

struct Node *vars()
{
	struct Node *varsProd = newNode("<vars>");

	if(tk->id == KW_tk && strcmp(tk->string, "declare") == 0)
	{
		struct Node *declareNode = newNode(tk->string);
		declareNode->id = tk->id;
		getNode(varsProd, declareNode);
		tk = fsa_driver();
		if(tk->id == ID_tk)
		{
			struct Node *idNode = newNode(tk->string);
			idNode->id = tk->id;
			getNode(varsProd, idNode);
			tk = fsa_driver();
			if(tk->id == OP_tk && strcmp(tk->string, ":=") == 0)
			{
				struct Node *opNode = newNode(tk->string);
				opNode->id = tk->id;
				getNode(varsProd, opNode);
				tk = fsa_driver();
				if(tk->id == NUM_tk)
				{
					struct Node *numNode = newNode(tk->string);
					numNode->id = tk->id;
					getNode(varsProd, numNode);
					tk = fsa_driver();
					if(tk->id == OP_tk && strcmp(tk->string, ";") == 0)
					{
						struct Node *closingNode = newNode(tk->string);
						closingNode->id = tk->id;
						getNode(varsProd, closingNode);

						tk = fsa_driver();
						struct Node *varsProd2 = vars();
						getNode(varsProd, varsProd2);
						return varsProd;
					}
					else
					{
						printf("Expected ; but got %s : %d\n", tk->string, tk->line);
						exit(1);
					}
				}
				else
				{
					printf("Expect Number but got %s : %d\n", tk->string, tk->line);
					exit(1);
				}
			}
			else
			{
				printf("Expected := but got %s : %d\n",tk->string, tk->line);
				exit(1);
			}
		}
		else
		{
			printf("Exepected identifier but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
	else
	{
		return varsProd;
	}
	return varsProd;
}

struct Node *block()
{
	struct Node *blockProd = newNode("<block>");

	if(tk->id == OP_tk && strcmp(tk->string, "{") == 0)
	{
		struct Node *lParNode = newNode(tk->string);
		lParNode->id = tk->id;
		getNode(blockProd, lParNode);

		tk = fsa_driver();
		struct Node *varsProd = vars();
		getNode(blockProd, varsProd);

		struct Node *statsProd = stats();
		getNode(blockProd, statsProd);

		if(tk->id == OP_tk && strcmp(tk->string, "}") == 0)
		{
			struct Node *closeParNode = newNode(tk->string);
			closeParNode->id = tk->id;
			getNode(blockProd, closeParNode);
			tk = fsa_driver();
			return blockProd;
		}
		else
		{
			printf("Expected '}' but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
}

struct Node *expr()
{
	struct Node *exprN = newNode("<expr>");

	if(tk->id == OP_tk || tk->id == ID_tk || tk->id == NUM_tk)
	{
		struct Node *nNode = N();
		getNode(exprN, nNode);
		if(tk->id == OP_tk && strcmp(tk->string, "-") == 0)
		{
			struct Node *minusNode = newNode(tk->string);
			minusNode->id = tk->id;
			getNode(exprN, minusNode);

			tk = fsa_driver();
			struct Node *exprNode2 = expr();
			getNode(exprN, exprNode2);
			return exprN;
		}
		
		return exprN;
	}
	else
	{
		printf("None of these tokens work %s\n", tk->string);
		exit(1);
	}
}

struct Node *N()
{
	struct Node *nProd = newNode("<N>");

	if(tk->id == OP_tk || tk->id == ID_tk || tk->id == NUM_tk)
	{
		struct Node *aNode = A();
		getNode(nProd, aNode);
		if(tk->id == OP_tk && strcmp(tk->string, "/") == 0)
		{
			struct Node *divNode = newNode(tk->string);
			divNode->id = tk->id;
			getNode(nProd, divNode);

			tk = fsa_driver();
			struct Node *nNode = N();
			getNode(nProd, nNode);
			return nProd;
		}

		else if(tk->id == OP_tk && strcmp(tk->string, "+") == 0)
		{
			struct Node *plusNode = newNode(tk->string);
			plusNode->id = tk->id;
			getNode(nProd, plusNode);

			tk = fsa_driver();
			struct Node *nNode2 = N();
			getNode(nProd, nNode2);
			return nProd;
		}
		return nProd;
	}
	else
	{
		printf("None of the tokens worked %s\n", tk->string);
		exit(1);
	}
}

struct Node *A()
{
	struct Node *aProd = newNode("<A>");

	if(tk->id == OP_tk || tk->id == ID_tk || tk->id == NUM_tk)
	{
		struct Node *mNode = M();
		getNode(aProd, mNode);
		if(tk->id == OP_tk && strcmp(tk->string, "+") == 0)
		{
			struct Node *plusNode = newNode(tk->string);
			plusNode->id = tk->id;
			getNode(aProd, plusNode);

			tk = fsa_driver();
			struct Node *aNode = A();
			getNode(aProd, aNode);
			return aProd;
		}
		return aProd;
	}
	else
	{
		printf("Not in the tokens %s\n", tk->string);
		exit(1);
	}

}


struct Node *M()
{
	struct Node *mProd = newNode("<M>");
	
	if(tk->id == OP_tk && strcmp(tk->string, "*") == 0)
	{
		struct Node *multNode = newNode(tk->string);
		multNode->id = tk->id;
		getNode(mProd, multNode);
		
		tk = fsa_driver();

		struct Node *mNode = M();
		getNode(mProd, mNode);
		return mProd;
	}
	
	if(tk->id == OP_tk && strcmp(tk->string, "(") == 0 || tk->id == ID_tk || tk->id == NUM_tk)
	{
		struct Node *rNode = R();
		getNode(mProd, rNode);
		return mProd;
	}
	else
	{
		printf("All dems tokens was wrong %s\n", tk->string);
		exit(1);
	}

}


struct Node *R()
{
	struct Node *rProd = newNode("<R>");
	
	if(tk->id == OP_tk && strcmp(tk->string, "(") == 0)
	{
		struct Node *openNode = newNode(tk->string);
		openNode->id = tk->id;
		getNode(rProd, openNode);
		

		tk = fsa_driver();
		struct Node *eNode = expr();
		getNode(rProd, eNode);
		if(tk->id == OP_tk && strcmp(tk->string, ")") == 0)
		{
			struct Node *closeNode = newNode(tk->string);
			closeNode->id = tk->id;
			getNode(rProd, closeNode);
			
			tk = fsa_driver();
			return rProd;
		}
		else
		{
			printf("OP token not closing parentheses %s\n", tk->string);
			exit(1);
		}
	}

	if(tk->id == ID_tk)
	{
		struct Node *idNode = newNode(tk->string);
		idNode->id = tk->id;
		getNode(rProd, idNode); 
		
		tk = fsa_driver();
		return rProd;
	}

	if(tk->id == NUM_tk)
	{
		struct Node *numNode = newNode(tk->string);
		numNode->id = tk->id;
		getNode(rProd, numNode);
		
		tk = fsa_driver();
		return rProd;
	}
	return rProd;
}

struct Node *stats()
{
	struct Node *statsProd = newNode("<stats>");
	
	struct Node *statNode = stat();
	getNode(statsProd, statNode);

	struct Node *mStatNode = mStat();
	getNode(statsProd, mStatNode);
	return statsProd;
}

struct Node *mStat()
{
	struct Node *mStatProd = newNode("<mStat>");
	
	if(tk->id == KW_tk)
	{
		struct Node *statNode = stat();
		getNode(mStatProd, statNode);

		struct Node *mStatNode = mStat();
		getNode(mStatProd, mStatNode);
		return mStatProd;
	}
	else
	{
		return mStatProd;
	}
}

struct Node *stat()
{
	struct Node *statProd = newNode("<stat>");
	
	if(tk->id == KW_tk && strcmp(tk->string, "in") == 0)
	{	
		struct Node *inNode = in();
		getNode(statProd, inNode);
		return statProd;
	}
	else if(tk->id == KW_tk && strcmp(tk->string, "out") == 0)
	{
		struct Node *outNode = out();
		getNode(statProd, outNode);
		return statProd;
	}
	else if(tk->id == OP_tk && strcmp(tk->string, "}") == 0 || tk->id == OP_tk && strcmp(tk->string, "{") == 0)
	{
		struct Node *blockNode = block();
		getNode(statProd, blockNode);
		return statProd;
	}
	else if(tk->id == KW_tk && strcmp(tk->string, "iffy") == 0)
	{
		struct Node *ifNode = iffy();
		getNode(statProd, ifNode);
		return statProd;
	}
	else if(tk->id == KW_tk && strcmp(tk->string, "loop") == 0)
	{
		struct Node *loopNode = loop();
		getNode(statProd, loopNode);
		return statProd;
	}
	else if(tk->id == ID_tk)
	{
		struct Node *assNode = assign();
		getNode(statProd, assNode);
		return statProd;
	}
	else if(tk->id == KW_tk && strcmp(tk->string, "goto") == 0)
	{
		struct Node *gotoNode = Goto();
		getNode(statProd, gotoNode);
		return statProd;
	}
	else if(tk->id == KW_tk && strcmp(tk->string, "label") == 0)
	{
		struct Node *labelNode = label();
		getNode(statProd, labelNode);
		return statProd;
	}
	else
	{
		printf("Error: no match %s\n", tk->string);
		exit(1);
	}
}

struct Node *in()
{
	struct Node *inProd = newNode("<in>");

	if(tk->id == KW_tk && strcmp(tk->string, "in") == 0)
	{
		struct Node *inNode = newNode(tk->string);
		inNode->id = tk->id;
		getNode(inProd, inNode);
		
		tk = fsa_driver();
		if(tk->id == ID_tk)
		{
			struct Node *idNode = newNode(tk->string);
			idNode->id = tk->id;
			getNode(inProd, idNode);
			
			tk = fsa_driver();
			if(tk->id == OP_tk && strcmp(tk->string, ";") == 0)
			{
				struct Node *parNode = newNode(tk->string);
				parNode->id = tk->id;
				getNode(inProd, parNode);
				
				tk = fsa_driver();
				return inProd;
			}
			else
			{
				printf("Expected ';' but got %s : %d\n", tk->string, tk->line);
				exit(1);
			}
		}
		else
		{
			printf("Expected identifier but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
	else
	{
		printf("Expected keyword 'in' but got %s : %d\n", tk->string, tk->line);
		exit(1);
	}
}

struct Node *out()
{	
	struct Node *outProd = newNode("<out>");
	
	if(tk->id == KW_tk && strcmp(tk->string, "out") == 0)
	{
		struct Node *outNode = newNode(tk->string);
		outNode->id = tk->id;
		getNode(outProd, outNode);
		

		tk = fsa_driver();
		struct Node *exprNode = expr();
		getNode(outProd, exprNode);

		if(tk->id == OP_tk && strcmp(tk->string, ";") == 0)
		{
			struct Node *semiNode = newNode(tk->string);
			semiNode->id = tk->id;
			getNode(outProd, semiNode);
			
			tk = fsa_driver();
			return outProd;
		}
		else
		{
			printf("Expected ';' but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
	else
	{
		printf("Expected keyword 'out' but got %s : %d\n", tk->string, tk->line);
		exit(1);
	}
}

struct Node *iffy()
{
	struct Node *ifProd = newNode("<if>");
	
	if(tk->id == KW_tk && strcmp(tk->string, "iffy") == 0)
	{
		struct Node *ifNode = newNode(tk->string);
		ifNode->id = tk->id;
		getNode(ifProd, ifNode);
	
		tk = fsa_driver();
		if(tk->id == OP_tk && strcmp(tk->string, "[") == 0)
		{ 
			struct Node *oBrackNode = newNode(tk->string);
			oBrackNode->id = tk->id;
			getNode(ifProd, oBrackNode);
			
			tk = fsa_driver();
			struct Node *expr1Node = expr();
			getNode(ifProd, expr1Node);

			struct Node *roNode = RO();
			getNode(ifProd, roNode);

			struct Node *expr2Node = expr();
			getNode(ifProd, expr2Node);
			if(tk->id == OP_tk && strcmp(tk->string, "]") == 0)
			{
				struct Node *cBrackNode = newNode(tk->string);
				cBrackNode->id = tk->id;
				getNode(ifProd, cBrackNode);
				
				tk = fsa_driver();
				if(tk->id == KW_tk && strcmp(tk->string, "then") == 0)
				{
					struct Node *keyNode = newNode(tk->string);
					keyNode->id = tk->id;
					getNode(ifProd, keyNode);
					
					tk = fsa_driver();
					struct Node *statNode = stat();
					getNode(ifProd, statNode);
					if(tk->id == OP_tk && strcmp(tk->string, ";") == 0)
					{
						struct Node *semiNode = newNode(tk->string);
						semiNode->id = tk->id;
						getNode(ifProd, semiNode);
					
						tk = fsa_driver();
						return ifProd;
					}
					else
					{
						printf("Expected ';' but got %s : %d\n", tk->string, tk->line);
						exit(1);
					}
				}
				else
				{
					printf("Expected Keyword 'then' but got %s : %d\n", tk->string, tk->line);
					exit(1);
				}
			}
			else
			{
				printf("Expected ']' but got %s : %d\n", tk->string, tk->line);
				exit(1);
			}
		}
		else
		{
			printf("Expected '[' but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
	else
	{
		printf("Expected keyword 'iffy' but got %s : %d\n", tk->string, tk->line);
		exit(1);
	}
}

struct Node *loop()
{
	struct Node *loopProd = newNode("<loop>");

	if(tk->id == KW_tk && strcmp(tk->string, "loop") == 0)
	{
		struct Node *loopNode = newNode(tk->string);
		loopNode->id = tk->id;
		getNode(loopProd, loopNode);

		tk = fsa_driver();
		if(tk->id == OP_tk && strcmp(tk->string, "[") == 0)
		{
			struct Node *oBrackNode = newNode(tk->string);
			oBrackNode->id = tk->id;
			getNode(loopProd, oBrackNode);
			
			tk = fsa_driver();
			struct Node *eNode1 = expr();
			getNode(loopProd, eNode1);

			struct Node *roNode = RO();
			getNode(loopProd, roNode);

			struct Node *eNode2 = expr();
			getNode(loopProd, eNode2);
			if(tk->id == OP_tk && strcmp(tk->string, "]") == 0)
			{
				struct Node *cBrackNode = newNode(tk->string);
				cBrackNode->id = tk->id;
				getNode(loopProd, cBrackNode);
				
				tk = fsa_driver();
				struct Node *statNode = stat();
				getNode(loopProd, statNode);
				if(tk->id == OP_tk && strcmp(tk->string, ";") == 0)
				{
					struct Node *semiNode = newNode(tk->string);
					semiNode->id = tk->id;
					getNode(loopProd, semiNode);
					
					tk = fsa_driver();
					return loopProd;
				}
				else
				{
					printf("Expected ';' but got %s : %d\n", tk->string, tk->line);
					exit(1);
				}
			}
			else
			{
				printf("Expected ']' but got %s : %d\n", tk->string, tk->line);
				exit(1);
			}
		}
		else
		{
			printf("Expected '[' but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
	else
	{
		printf("Expected keyword 'loop' but got %s : %d\n", tk->string, tk->line);
		exit(1);
	}
}

struct Node *assign()
{
	struct Node *assProd = newNode("<assign>");
	
	if(tk->id == ID_tk)
	{
		struct Node *idNode = newNode(tk->string);
		idNode->id = tk->id;
		getNode(assProd, idNode);
		
		tk = fsa_driver();
		if(tk->id == OP_tk && strcmp(tk->string, ":=") == 0)
		{
			struct Node *assNode = newNode(tk->string);
			assNode->id = tk->id;
			getNode(assProd, assNode);
			
			tk = fsa_driver();
			struct Node *exprNode = expr();
			getNode(assProd, exprNode);
			if(tk->id == OP_tk && strcmp(tk->string, ";") == 0)
			{
				struct Node *semiNode = newNode(tk->string);
				semiNode->id = tk->id;
				getNode(assProd, semiNode);
				
				tk = fsa_driver();
				return assProd;
			}
			else
			{
				printf("Expected ';' but got %s : %d\n", tk->string, tk->line);
				exit(1);
			}
		}
		else
		{
			printf("Expected ':=' but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
	else
	{
		printf("Expected ID token bt got %s : %d\n", tk->string, tk->line);
		exit(1);
	}
}

struct Node *label()
{
	struct Node *labelProd = newNode("<label>");

	if(tk->id == KW_tk && strcmp(tk->string, "label") == 0)
	{
		struct Node *labNode = newNode(tk->string);
		labNode->id = tk->id;
		getNode(labelProd, labNode);
		
		tk = fsa_driver();
		if(tk->id == ID_tk)
		{
			struct Node *idNode = newNode(tk->string);
			idNode->id = tk->id;
			getNode(labelProd, idNode);
			
			tk = fsa_driver();
			if(tk->id == OP_tk && strcmp(tk->string, ";") == 0)
			{
				struct Node *parNode = newNode(tk->string);
				parNode->id = tk->id;
				getNode(labelProd, parNode);
			
				tk = fsa_driver();
				return labelProd;
			}
			else
			{
				printf("Expected ';' but got %s : %d\n", tk->string, tk->line);
				exit(1);
			}
		}
		else
		{
			printf("Expexted identifier but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
	else
	{
		printf("Expected keyword 'label' but got %s : %d\n", tk->string, tk->line);
		exit(1);
	}
}

//goto statements
struct Node *Goto()
{
	struct Node *gotoProd = newNode("<goto>");
	
	if(tk->id == KW_tk && strcmp(tk->string, "goto") == 0)
	{
		struct Node *goNode = newNode(tk->string);
		goNode->id = tk->id;
		getNode(gotoProd, goNode);
		
		tk = fsa_driver();
		if(tk->id == ID_tk)
		{
			struct Node *idNode = newNode(tk->string);
			idNode->id = tk->id;
			getNode(gotoProd, idNode);
			
			tk = fsa_driver();
			if(tk->id == OP_tk && strcmp(tk->string, ";") == 0)
			{
				struct Node *semiNode = newNode(tk->string);
				semiNode->id = tk->id;
				getNode(gotoProd, semiNode);
				
				tk = fsa_driver();
				return gotoProd;
			}
			else
			{
				printf("Expected ';' but got %s : %d\n", tk->string, tk->line);
				exit(1);
			}
		}
		else
		{
			printf("Expected identifier but got %s : %d\n", tk->string, tk->line);
			exit(1);
		}
	}
	else
	{
		printf("Expected keyword goto %s : %d\n", tk->string, tk->line);
		exit(1);
	}
}

//relational operators
struct Node *RO()
{
	struct Node *roProd = newNode("<RO>");
	
	if(tk->id == OP_tk && strcmp(tk->string, "<") == 0)
	{
		struct Node *opNode = newNode(tk->string);
		opNode->id = tk->id;
		getNode(roProd, opNode);
		
		tk = fsa_driver();
		if(tk->id == OP_tk && strcmp(tk->string, "<") == 0 || tk->id == OP_tk && strcmp(tk->string, ">") == 0)
		{
			struct Node *nextNode = newNode(tk->string);
			nextNode->id = tk->id;
			getNode(roProd, nextNode);
			//double <<
			
			tk = fsa_driver();
			return roProd;
		}
		else
		{
			//single <
			return roProd;
		}
	}
	else if(tk->id == OP_tk && strcmp(tk->string, ">") == 0)
	{
		struct Node *nextNode2 = newNode(tk->string);
		nextNode2->id = tk->id;
		getNode(roProd, nextNode2);
		
		tk = fsa_driver();
		if(tk->id == OP_tk && strcmp(tk->string, ">") == 0)
		{
			struct Node *nextNode3 = newNode(tk->string);
			nextNode3->id = tk->id;
			getNode(roProd, nextNode3);
			//double >>
		
			tk = fsa_driver();
			return roProd;
		}
		else
		{
			//single >
			return roProd; 
		}
	}
	else if(tk->id == OP_tk && strcmp(tk->string, "==") == 0)
	{
		struct Node *equalNode = newNode(tk->string);
		equalNode->id = tk->id;
		getNode(roProd, equalNode);
		//double == 
	
		tk = fsa_driver();
		return roProd;
	}
	else 
	{
		struct Node *assNode = newNode(tk->string);
		assNode->id = tk->id;
		getNode(roProd, assNode);
		//single =
		
		tk = fsa_driver();
		return roProd;
	}
}
