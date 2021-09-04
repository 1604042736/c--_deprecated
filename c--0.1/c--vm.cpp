#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <vector>
#include <iostream>
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define   DADDR_SIZE  1024
#define   NO_REGS 8
#define   PC_REG  7

#define   LINESIZE  121
#define   WORDSIZE  20

typedef enum 
{
   	opclRR,
   	opclRM,
   	opclRA
} OPCLASS;

typedef enum 
{
   	opHALT,
   	opIN, 
   	opOUT, 
   	opADD,
   	opSUB,
   	opMUL,
   	opDIV, 
   	opRDE,
   	opRRLim,
	
   	opLD,
   	opST,
	opRMLim,

   	opLDA,
   	opLDC,
   	opJLT,
   	opJLE,
   	opJGT,
   	opJGE,
   	opJEQ,
   	opJNE,
   	opAND,
   	opOR,
   	opRET,
   	opCALL,
   	opSTP,
   	opLDP,
   	opASTP,
   	opPOP,
   	opBACK,
   	opSTNA,
   	opNEWA,
   	opSTA,
   	opALDA,
   	opRALim
} OPCODE;

typedef enum 
{
   	srOKAY,
   	srHALT,
   	srIMEM_ERR,
   	srDMEM_ERR,
   	srZERODIVIDE
} STEPRESULT;

typedef struct 
{
	int iop  ;
    int iarg1  ;
    double iarg2  ;
    int iarg3  ;
} INSTRUCTION;

int IADDR_SIZE;

INSTRUCTION* iMem;
double dMem [DADDR_SIZE];
double reg [NO_REGS];
vector<int> ret;
vector<double> pMem;
vector<double> pop;
vector<double> params;
vector<double*> array;
vector<int> arraynum;

char * opCodeTab[]
        = {(char*)"HALT",(char*)"IN",(char*)"OUT",(char*)"ADD",(char*)"SUB",(char*)"MUL",(char*)"DIV",(char*)"RDE",(char*)"????",
           (char*)"LD",(char*)"ST",(char*)"????",
           (char*)"LDA",(char*)"LDC",(char*)"JLT",(char*)"JLE",(char*)"JGT",(char*)"JGE",
		   (char*)"JEQ",(char*)"JNE",(char*)"AND",(char*)"OR",(char*)"RET",(char*)"CALL",
		   (char*)"STP",(char*)"LDP",(char*)"ASTP",(char*)"POP",(char*)"BACK",(char*)"STNA",(char*)"NEWA",(char*)"STA",
		   (char*)"ALDA",(char*)"????"
          };

FILE *pgm  ;

char in_Line[LINESIZE] ;
int lineLen ;
int inCol  ;
int num  ;
char word[WORDSIZE] ;
char ch  ;

int opClass( int c )
{ 
	if( c <= opRRLim) 
		return ( opclRR );
  	else if ( c <= opRMLim) 
		return ( opclRM );
  	else
	  return ( opclRA );
}

void getCh (void)
{ 
	if (++inCol < lineLen)
  		ch = in_Line[inCol] ;
  	else 
	  	ch = ' ' ;
}

int nonBlank (void)
{ 
	while ((inCol < lineLen)&& (in_Line[inCol] == ' ') )
    	inCol++ ;
  	if (inCol < lineLen)
  	{ 
	  	ch = in_Line[inCol] ;
    	return TRUE ; 
	}
  	else
  	{ 
	  	ch = ' ' ;
    	return FALSE ; 
	}
}

int getNum (void)
{ 
	int sign;
  	int term;
  	int temp = FALSE;
  	num = 0 ;
  	do
  	{ 
	  	sign = 1;
  	  	while ( nonBlank() && ((ch == '+') || (ch == '-')) )
  	  	{ 
			temp = FALSE ;
  	  	  	if (ch == '-')  sign = - sign ;
  	  	  	getCh();
  	  	}
  	  	term = 0 ;
  	  	nonBlank();
  	  	while (isdigit(ch))
  	  	{ 
			temp = TRUE ;
  	  	  	term = term * 10 + ( ch - '0' ) ;
  	  	  	getCh();
  	  	}
  	  	num = num + (term * sign) ;
  	} while ( (nonBlank()) && ((ch == '+') || (ch == '-')) ) ;
  	return temp;
}

int getWord (void)
{ 
	int temp = FALSE;
 	int length = 0;
  	if (nonBlank ())
  	{ 
	  	while (isalnum(ch))
  	  	{ 
			if (length < WORDSIZE-1) 
				word [length++] =  ch ;
  	  	  	getCh() ;
  	  	}
  	  	word[length] = '\0';
  	  	temp = (length != 0);
  	}
  	return temp;
}

int skipCh ( char c  )
{ 
	int temp = FALSE;
  	if ( nonBlank() && (ch == c) )
  	{ 
	  	getCh();
  	  	temp = TRUE;
  	}
  	return temp;
}

int error( char * msg, int lineNo, int instNo)
{ 
	printf("Line %d",lineNo);
  	if (instNo >= 0) 
	  printf(" (Instruction %d)",instNo);
  	printf("   %s\n",msg);
  	return FALSE;
}

OPCODE operator++(OPCODE &p,int)
{
	switch(p)
	{
		case opHALT:
			p=opIN;
			break;
		case opIN:
			p=opOUT;
			break;
		case opOUT:
			p=opADD;
			break;
		case opADD:
			p=opSUB;
			break;
		case opSUB:
			p=opMUL;
			break;
		case opMUL:
			p=opDIV;
			break;
		case opDIV:
			p=opRDE;
			break;
		case opRDE:
			p=opRRLim;
			break;
		case opRRLim:
			p=opLD;
			break;
		case opLD:
			p=opST;
			break;
		case opST:
			p=opRMLim;
			break;
		case opRMLim:
			p=opLDA;
			break;
		case opLDA:
			p=opLDC;
			break;
		case opLDC:
			p=opJLT;
			break;
		case opJLT:
			p=opJLE;
			break;
		case opJLE:
			p=opJGT;
			break;
		case opJGT:
			p=opJGE;
			break;
		case opJGE:
			p=opJEQ;
			break;
		case opJEQ:
			p=opJNE;
			break;
		case opJNE:
			p=opAND;
			break;
		case opAND:
			p=opOR;
			break;
		case opOR:
			p=opRET;
			break;
		case opRET:
			p=opCALL;
			break;
		case opCALL:
			p=opSTP;
			break;
		case opSTP:
			p=opLDP;
			break;
		case opLDP:
			p=opASTP;
			break;
		case opASTP:
			p=opPOP;
			break;
		case opPOP:
			p=opBACK;
			break;
		case opBACK:
			p=opSTNA;
			break;
		case opSTNA:
			p=opNEWA;
			break;
		case opNEWA:
			p=opSTA;
			break;
		case opSTA:
			p=opALDA;
			break;
		case opALDA:
			p=opRALim;
			break;
		case opRALim:
			p=opHALT;
			break;
	}
}

int readInstructions (void)
{ 
	OPCODE op;
  	int arg1, arg2, arg3;
  	int loc, regNo, lineNo;
  	for (regNo = 0 ; regNo < NO_REGS ; regNo++)
  	    reg[regNo] = 0 ;
  	dMem[0] = DADDR_SIZE - 1 ;
  	for (loc = 1 ; loc < DADDR_SIZE ; loc++)
  	    dMem[loc] = 0 ;
  	for (loc = 0 ; loc < IADDR_SIZE ; loc++)
  	{ 
	  	iMem[loc].iop = opHALT ;
  	  	iMem[loc].iarg1 = 0 ;
  	  	iMem[loc].iarg2 = 0 ;
  	  	iMem[loc].iarg3 = 0 ;
  	}
  	lineNo = 0 ;
  	while (! feof(pgm))
  	{ 
	  	fgets( in_Line, LINESIZE-2, pgm  ) ;
  	  	inCol = 0 ; 
  	  	lineNo++;
  	  	lineLen = strlen(in_Line)-1 ;
  	  	if (in_Line[lineLen]=='\n') 
			in_Line[lineLen] = '\0' ;
  	  	else 
			in_Line[++lineLen] = '\0';
  	  	if ( (nonBlank()) && (in_Line[inCol] != '*') )
  	  	{ 
			if (! getNum())
        		return error((char*)"Bad location", lineNo,-1);
   	   		loc = num;
   	   		if (loc > IADDR_SIZE)
   	     		return error((char*)"Location too large",lineNo,loc);
   	   		if (! skipCh(':'))
   	     		return error((char*)"Missing colon", lineNo,loc);
   	   		if (! getWord ())
   	     		return error((char*)"Missing opcode", lineNo,loc);
   	   		op = opHALT ;
   	   		while ((op < opRALim)&& (strncmp(opCodeTab[op], word, 4) != 0) )
   	       		op++ ;
   	   		if (strncmp(opCodeTab[op], word, 4) != 0)
   	       		return error((char*)"Illegal opcode", lineNo,loc);
   	   		switch ( opClass(op) )
   	   		{ 
				case opclRR :
   	     			if ( (! getNum ()) || (num < 0) || (num >= NO_REGS) )
   	        	 		return error((char*)"Bad first register", lineNo,loc);
   	     			arg1 = num;
   	     			if ( ! skipCh(','))
   	         			return error((char*)"Missing comma", lineNo, loc);
   		     		if ( (! getNum ()) || (num < 0) || (num >= NO_REGS) )
   	         			return error((char*)"Bad second register", lineNo, loc);
   	     			arg2 = num;
   	     			if ( ! skipCh(',')) 
   	         			return error((char*)"Missing comma", lineNo,loc);
   	     			if ( (! getNum ()) || (num < 0) || (num >= NO_REGS) )
   	         			return error((char*)"Bad third register", lineNo,loc);
   	     			arg3 = num;
    	    		break;		

        		case opclRM :
        		case opclRA :
        			if ( (! getNum ()) || (num < 0) || (num >= NO_REGS) )
            			return error((char*)"Bad first register", lineNo,loc);
        			arg1 = num;
        			if ( ! skipCh(','))
            			return error((char*)"Missing comma", lineNo,loc);
        			if (! getNum ())
            			return error((char*)"Bad displacement", lineNo,loc);
        			arg2 = num;
        			if ( ! skipCh('(') && ! skipCh(',') )
            			return error((char*)"Missing LParen", lineNo,loc);
        			if ( (! getNum ()) || (num < 0) || (num >= NO_REGS))
            			return error((char*)"Bad second register", lineNo,loc);
        			arg3 = num;
        			break;
        	}
      		iMem[loc].iop = op;
      		iMem[loc].iarg1 = arg1;
      		iMem[loc].iarg2 = arg2;
      		iMem[loc].iarg3 = arg3;
    	}
  	}
  	return TRUE;
}

STEPRESULT stepTM (void)
{ 
	INSTRUCTION currentinstruction  ;
  	int pc  ;
  	int r,s,t,m  ;
  	double ok ;
	
	pc = reg[PC_REG] ;
  	if ( (pc < 0) || (pc > IADDR_SIZE)  )
      	return srIMEM_ERR ;
  	reg[PC_REG] = pc + 1 ;
  	currentinstruction = iMem[ pc ] ;
  	switch (opClass(currentinstruction.iop) )
  	{ 
	  	case opclRR :
      		r = currentinstruction.iarg1 ;
      		s = currentinstruction.iarg2 ;
      		t = currentinstruction.iarg3 ;
      		break;

    	case opclRM :
      		r = currentinstruction.iarg1 ;
      		s = currentinstruction.iarg3 ;
      		m = currentinstruction.iarg2 + reg[s] ;
      		if ( (m < 0) || (m > DADDR_SIZE))
         		return srDMEM_ERR ;
      		break;

    	case opclRA :
      		r = currentinstruction.iarg1 ;
      		s = currentinstruction.iarg3 ;
      		m = currentinstruction.iarg2 + reg[s] ;
      		break;
  	}
  	double* db;
  	switch ( currentinstruction.iop)
  	{
		case opHALT :
      		return srHALT ;

    	case opIN :
      		do
      		{ 
        		fflush (stdin);
        		fflush (stdout);
        		gets(in_Line);
        		lineLen = strlen(in_Line) ;
        		inCol = 0;
        		ok = getNum();
        		if (  ok ) 
					reg[r] = num;
      		}while (! ok);
      		break;

    	case opOUT :  
    		cout<<reg[r];
      		break;
    	case opADD :  
			reg[r] = reg[s] + reg[t] ;  
			break;
    	case opSUB :  
			reg[r] = reg[s] - reg[t] ;  
			break;
    	case opMUL :  
			reg[r] = reg[s] * reg[t] ;  
			break;

    	case opDIV :
      		if ( reg[t] != 0 ) 
			  	reg[r] = reg[s] / reg[t];
      		else 
			  return srZERODIVIDE ;
      		break;
      	case opRDE:
      		if ( reg[t] != 0 ) 
			  	reg[r] = (int)reg[s] % (int)reg[t];
      		else 
			  return srZERODIVIDE ;
      		break;
    	case opLD :    
			reg[r] = dMem[m] ;  
			break;
    	case opST :
			dMem[m] = reg[r] ;  
			break;
    	case opLDA :    
			reg[r] = m ; 
			break;
    	case opLDC :
			reg[r] = currentinstruction.iarg2 ;   
			break;
    	case opRET:
    		for(int i=0;i<currentinstruction.iarg2;i++)
			{
    			pMem.erase(pMem.begin());
    		}
    		reg[r]=ret.back()+1;
    		ret.pop_back();
    		break;
    	case opCALL:
    		for(int i=0;i<currentinstruction.iarg3;i++)
    		{
    			pMem.insert(pMem.begin(),params.back());
    			params.pop_back();
    		}
    		reg[r]=currentinstruction.iarg2;
    		ret.push_back(pc);
    		break;
    	case opSTP:
    		params.push_back(reg[r]);
    		break;
    	case opLDP:
    		reg[r]=pMem[currentinstruction.iarg2];
    		break;
    	case opASTP:
    		pMem[currentinstruction.iarg2]=reg[r];
    		break;
    	case opPOP:
    		pop.push_back(dMem[m]);
    		break;
    	case opBACK:
    		dMem[m]=pop.back();
    		pop.pop_back();
    		break;
    	case opSTNA:
    		arraynum.push_back(reg[r]);
    		break;
    	case opNEWA:
    		db=new double[arraynum.back()];
    		arraynum.pop_back();
    		array.push_back(db);
    		break;
    	case opSTA:
    		array[(int)currentinstruction.iarg2][arraynum.back()]=reg[r];
    		arraynum.pop_back();
    		break;
    	case opALDA:
    		reg[r]=array[(int)currentinstruction.iarg2][arraynum.back()];
    		arraynum.pop_back();
    		break;
    	case opJLT :    if ( reg[r] <  0 ) reg[PC_REG] = m ; break;
    	case opJLE :    if ( reg[r] <=  0 ) reg[PC_REG] = m ; break;
    	case opJGT :    if ( reg[r] >  0 ) reg[PC_REG] = m ; break;
    	case opJGE :    if ( reg[r] >=  0 ) reg[PC_REG] = m ; break;
    	case opJEQ :    if ( reg[r] == 0 ) reg[PC_REG] = m ; break;
    	case opJNE :    if ( reg[r] != 0 ) reg[PC_REG] = m ; break;
    	case opAND: 	if(reg[r]!=0) 	reg[PC_REG]=m;	break;
    	case opOR:  	if(reg[r]!=0) 	reg[PC_REG]=m;	break;
  	}
  	return srOKAY ;
}

int doCommand (void)
{ 
	char cmd;
  	int stepcnt=0, i;
  	int printcnt;
  	int stepResult;
  	int regNo, loc;
  	stepResult = srOKAY;
  	while (stepResult == srOKAY)
    {
        stepResult = stepTM ();
        stepcnt++;
    }
  	return TRUE;
}

void getLine(char* filename)
{
	FILE* fp=fopen(filename,"r");
	int i=0;
	char line[256]={0};
    while(fgets(line,255,fp))
    {
        i++;
    }
    IADDR_SIZE=i;
    iMem=new INSTRUCTION[IADDR_SIZE];
    fclose(fp);
}

int main( int argc, char * argv[] )
{
	getLine((char*)"in.c--exe");
  	pgm = fopen("in.c--exe", "r");
  	if (pgm == NULL)
  	{ 
  		printf("file '%s' not found\n");
  	  	exit(1);
  	}
	
  	if ( ! readInstructions ())
    	exit(1) ;
  	doCommand ();
  	return 0;
}
