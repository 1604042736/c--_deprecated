#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<ctype.h>
#include<iostream>
#include<vector>
using namespace std;

#define FALSE 0
#define TRUE 1

#define MAXCHILDREN 3
#define SHIFT 4
#define MAXRESERVED 14
#define MAXTOKENLEN 40
#define SIZE 211
#define BUFLEN 256

#define ac 0
#define ac1 1
#define ord 2
#define orp 3
#define gp 5
#define mp 6
#define pc 7

typedef enum 
{
	ENDFILE,ERROR,
	IF,ELSE,DO,FOR,WHILE,BREAK,CONTINUE,READ,WRITE,INCLUDE,FUNC,RETURN,DEFINE,PRINT,INPUT,ARRAY,
	ID,NUM,STRING,
	ASSIGN,EQ,LT,GT,LTD,GTD,NOTD,PLUS,MINUS,TIMES,YTIMES,OVER,LPARAM,RPARAM,SEMI,LBRACE,RBRACE,SIM,LBRACKET,RBRACKET,
	AND,OR
} TokenType;

typedef enum 
{
	StmtK,ExpK
} NodeKind;

typedef enum 
{
	IfK,ForK,WhileK,DoWhileK,AssignK,ReadK,WriteK,IncludeK,FuncK,ParamK,ElseK,CallK,CallParamK,ReturnK,DefineK,
	DefineAssignK,PrintK,InputK,NoHandK,NewArrayK,ArrayAssignK,ArrayK,ArrayNumK
} StmtKind;

typedef enum 
{
	OpK,ConstK,IdK,StringK
} ExpKind;

typedef enum 
{
	Void,Integer,Boolean
} ExpType;

typedef enum 
{
	START,INASSIGN,INCOMMENT,INNUM,INID,DONE,INSTRING,INAND,INOR
} StateType;

static struct 
{
	char* str;
	TokenType tok;
} reservedWords[MAXRESERVED]= 
{
	{(char*)"if",IF},
	{(char*)"else",ELSE},
	{(char*)"do",DO},
	{(char*)"for",FOR},
	{(char*)"while",WHILE},
	{(char*)"break",BREAK},
	{(char*)"continue",CONTINUE},
	{(char*)"input",INPUT},
	{(char*)"print",PRINT},
	{(char*)"include",INCLUDE},
	{(char*)"func",FUNC},
	{(char*)"return",RETURN},
	{(char*)"define",DEFINE},
	{(char*)"array",ARRAY}
};

typedef struct
{
	char* name;
	char* val;
}define_list;
vector<define_list> definel;

typedef struct
{
	int lineno;
	char lineBuf[BUFLEN];
	int linepos;
	int bufsize;
	int EOF_FLAG;
	FILE* source;
	char* filename;
	char tokenString[MAXTOKENLEN + 1];
} Lexer;

typedef struct 
{
	Lexer* lexer;
	int error;
	TokenType token;
	TokenType lastToken;
} Parser;

typedef struct treeNode 
{
	struct treeNode* child[MAXCHILDREN];
	struct treeNode* sibling;
	int lineno;
	NodeKind nodekind;
	union 
	{ 
		StmtKind stmt; 
		ExpKind exp; 
	} kind;
	union 
	{ 
		TokenType op;
		int val;
		char* name;
	} attr;
	ExpType type;
} TreeNode;

typedef struct LineListRec 
{
	int lineno;
	struct LineListRec* next;
}* LineList;

typedef struct BucketListRec 
{
	char* name;
	LineList lines;
	int memloc;
	struct BucketListRec* next;
}* BucketList;

typedef struct 
{
	BucketList hashTable[SIZE];
} Symtab;

typedef struct 
{
	Symtab* symtab;
	int location;
	int error;
} Analyzer;

typedef struct
{
	char* name;
	int way;
	int params;
}cod;

typedef struct 
{
	int emitLoc;
	int highEmitLoc;
	FILE* out;
	int traceCode;
	int tmpOffset;
	Symtab* symtab;
	vector<cod> func;
	vector<cod> call;
	vector<cod> param;
	vector<cod> array;
	char* nowfunc;
	int params;
	int arrays;
} Coder;

void lexer_init(Lexer* lexer, FILE* source,char *filename) 
{
	lexer->lineno = 0;
	lexer->bufsize = 0;
	lexer->linepos = 0;
	lexer->EOF_FLAG = FALSE;
	lexer->source = source;
	lexer->filename=filename;
}

int find_define(char c[])
{
	for(int i=0;i<definel.size();i++)
	{
		if(!strcmp(c, definel[i].name))
		{
			return i;
		}
	}
	return -1;
}

void define_swap(char a[])
{
	char c[1024];
	int lenc=0;
	int pos=0;
	for(int i=0;i<strlen(a);i++)
	{
		if(isalpha(a[i])&&lenc==0)
		{
			pos=i;
		}
		if(isalpha(a[i]))
		{
			c[lenc++]=a[i];
		}
		else
		{
			c[lenc]='\0';
			int k=find_define(c);
			if(k!=-1)
			{
				string s=a;
				s.replace(pos,strlen(definel[k].name),definel[k].val);
				for(int j=0; j<s.length(); j++)
    			{
        			a[j] = s[j];
    			}
    			i=0;
    			a[s.length()]='\0';
			}
			lenc=0;
		}
	}
}

static int getNextChar(Lexer* lexer) 
{
	if (lexer->linepos < lexer->bufsize) 
	{
		return lexer->lineBuf[lexer->linepos++];
	}
	else 
	{
		lexer->lineno++;
		if(fgets(lexer->lineBuf, BUFLEN - 1, lexer->source))
		{
			if(lexer->lineBuf[0]!='#')
			{
				define_swap(lexer->lineBuf);
			}
			lexer->bufsize = strlen(lexer->lineBuf);
			lexer->linepos = 0;
			return lexer->lineBuf[lexer->linepos++];
		}
		else 
		{
			lexer->EOF_FLAG = TRUE;
			return EOF;
		}
	}
}

static void ungetNextChar(Lexer* lexer) 
{
	if (!lexer->EOF_FLAG) 
	{
		lexer->linepos--;
	}
}

static TokenType reservedLookup(char* s) 
{
	int i;
	for (i = 0; i < MAXRESERVED; i++) 
	{
		if (!strcmp(s, reservedWords[i].str)) 
		{
			return reservedWords[i].tok;
		}
	}
	return ID;
}

TokenType lexer_getToken(Lexer* lexer) 
{
	int tokenStringIndex = 0;
	TokenType currentToken;
	StateType state = START;
	int save;
	while (state != DONE) 
	{
		int c = getNextChar(lexer);
		save = TRUE;
		switch (state) 
		{
		case START:
			if (isdigit(c)) 
			{
				state = INNUM;
			}
			else if (isalpha(c)) 
			{
				state = INID;
			}
			else if(c=='"')
			{
				save=FALSE;
				state=INSTRING;
			}
			else if(c=='#')
			{
				save=FALSE;
				state=INID;
			}
			else if(c=='|')
			{
				state=INOR;
			}
			else if(c=='&')
			{
				state=INAND;
			}
			else if (c == '='||c=='>'||c=='<'||c=='!') 
			{
				state = INASSIGN;
			}
			else if ((c == ' ' || c == '\t' || c == '\n')) 
			{
				save = FALSE;
			}
			else if (c == '$') 
			{
				save = FALSE;
				state = INCOMMENT;
			}
			else 
			{
				state = DONE;
				switch (c) 
				{
				case EOF:
					save = FALSE;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '%':
					currentToken = YTIMES;
					break;
				case '/':
					currentToken = OVER;
					break;
				case '(':
					currentToken = LPARAM;
					break;
				case ')':
					currentToken = RPARAM;
					break;
				case ';':
					currentToken = SEMI;
					break;
				case '{':
					currentToken = LBRACE;
					break;
				case '}':
					currentToken = RBRACE;
					break;
				case '[':
					currentToken = LBRACKET;
					break;
				case ']':
					currentToken = RBRACKET;
					break;
				case ',':
					currentToken=SIM;
					break;
				default:
					currentToken = ERROR;
					break;
				}
			}
			break;
		case INCOMMENT:
			save = FALSE;
			if (c == EOF) 
			{
				state = DONE;
				currentToken = ENDFILE;
			}
			else if (c == '\n') 
			{
				state = START;
			}
			break;
		case INASSIGN:
			state = DONE;
			if (c == '=') 
			{
				char s=lexer->tokenString[tokenStringIndex-1];
				if(s=='=')
				{
					currentToken = EQ;
				}
				else if(s=='>')
				{
					currentToken = GTD;
				}
				else if(s=='<')
				{
					currentToken = LTD;
				}
				else if(s=='!')
				{
					currentToken = NOTD;
				}
			}
			else 
			{
				char s=lexer->tokenString[tokenStringIndex-1];
				ungetNextChar(lexer);
				save = FALSE;
				if(s=='=')
				{
					currentToken = ASSIGN;
				}
				else if(s=='>')
				{
					currentToken = GT;
				}
				else if(s=='<')
				{
					currentToken = LT;
				}
			}
			break;
			
		case INSTRING:
			if(c=='"')
			{
				save=FALSE;
				state=DONE;
				currentToken=STRING;
			}
			break;

		case INNUM:
			if (!isdigit(c)) 
			{
				ungetNextChar(lexer);
				save = FALSE;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c)) 
			{
				ungetNextChar(lexer);
				save = FALSE;
				state = DONE;
				currentToken = ID;
			}
			break;
		case INAND:
			if(c=='&')
			{
				state=DONE;
				currentToken=AND;
			}
			else
			{
				ungetNextChar(lexer);
				state=DONE;
				save=FALSE;
				currentToken=ERROR;
			}
			break;
		case INOR:
			if(c=='|')
			{
				state=DONE;
				currentToken=OR;
			}
			else
			{
				ungetNextChar(lexer);
				state=DONE;
				save=FALSE;
				currentToken=ERROR;
			}
			break;
		case DONE:
		default:
			state = DONE;
			currentToken = ERROR;
			break;
		}
		if (save && tokenStringIndex <= MAXTOKENLEN) 
		{
			lexer->tokenString[tokenStringIndex++] = (char)c;
		}
		if (state == DONE) 
		{
			lexer->tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID) 
			{
				currentToken = reservedLookup(lexer->tokenString);
			}
		}
	}
	return currentToken;
}

void parser_init(Parser* parser,Lexer* lexer) 
{
	parser->lexer = lexer;
	parser->error = FALSE;
}

void printToken(TokenType token, const char* tokenString) 
{
	switch (token) 
	{
	case IF:
	case ELSE:
	case DO:
	case FOR:
	case WHILE:
	case BREAK:
	case CONTINUE:
	case READ:
	case WRITE:
	case PRINT:
	case INPUT:
		printf("reserved word: %s\n", tokenString);
		break;
	case ASSIGN:
		printf("=\n");
		break;
	case EQ:
		printf("==\n");
		break;
	case LT:
		printf("<\n");
		break;
	case GT:
		printf(">\n");
		break;
	case LTD:
		printf("<=\n");
		break;
	case GTD:
		printf(">=\n");
		break;
	case NOTD:
		printf("!=\n");
		break;
	case OR:
		printf("||\n");
		break;
	case AND:
		printf("&&\n");
		break;
	case PLUS:
		printf("+\n");
		break;
	case MINUS:
		printf("-\n");
		break;
	case TIMES:
		printf("*\n");
		break;
	case YTIMES:
		cout<<"%\n";
		break;
	case OVER:
		printf("/\n");
		break;
	case LPARAM:
		printf("(\n");
		break;
	case RPARAM:
		printf(")\n");
		break;
	case SEMI:
		printf(";\n");
		break;
	case SIM:
		printf(",\n");
		break;
	case LBRACE:
		printf("{\n");
		break;
	case RBRACE:
		printf("}\n");
		break;
	case ENDFILE:
		printf("EOF\n");
		break;
	case RETURN:
		printf("return\n");
		break;
	case NUM:
		printf("NUM, val= %s\n", tokenString);
		break;
	case ID:
		printf("ID, name= %s\n", tokenString);
		break;
	case ARRAY:
		printf("array\n");
		break;
	case ERROR:
		printf("ERROR : %s\n", tokenString);
		break;
	default:
		break;
		
	}
}

TreeNode* newStmtNode(StmtKind kind,int lineno) 
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL) 
	{
		printf("out of memory at line %d\n", lineno);
	}
	else 
	{
		for (i = 0; i < MAXCHILDREN; i++) 
		{
			t->child[i] = NULL;
		}
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

TreeNode* newExpNode(ExpKind kind, int lineno) 
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL) 
	{
		printf("out of memory at line %d\n",lineno);
	}
	else {
		for (i = 0; i < MAXCHILDREN; i++) 
		{
			t->child[i] = NULL;
		}
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}
	return t;
}

char* copyString(char* s) 
{
	int n;
	char* t;
	if (s == NULL) 
	{
		return NULL;
	}
	n = strlen(s) + 1;
	t = (char*)malloc(n);
	if (t == NULL) 
	{
		return NULL;
	}
	else 
	{
		strcpy(t, s);
	}
	return t;
}

static void printSpaces(int indentno) 
{
	for (int i = 0; i < indentno; i++) 
	{
		printf(" ");
	}
}

static void _printTree(TreeNode* tree, int indentno) 
{
	int i;
	indentno += 2;
	while (tree != NULL) 
	{
		printSpaces(indentno);
		if (tree->nodekind == StmtK) 
		{
			switch (tree->kind.stmt) 
			{
			case IfK:
				printf("If\n");
				break;
			case ElseK:
				printf("Else\n");
				break;
			case ForK:
				printf("For\n");
				break;
			case WhileK:
				printf("While\n");
				break;
			case DoWhileK:
				printf("DoWhile\n");
				break;
			case AssignK:
				printf("Assign to : %s\n", tree->attr.name);
				break;
			case ReadK:
				printf("read: %s\n", tree->attr.name);
				break;
			case InputK:
				printf("Input\n");
				break;
			case WriteK:
				printf("write\n");
				break;
			case PrintK:
				printf("Print\n");
				break;
			case IncludeK:
				printf("Include: %s\n",tree->attr.name);
				break;
			case FuncK:
				printf("Func: %s\n",tree->attr.name);
				break;
			case ParamK:
				printf("Param: %s\n",tree->attr.name);
				break;
			case CallK:
				printf("Call: %s\n",tree->attr.name);
				break;
			case CallParamK:
				printf("Param\n");
				break;
			case ReturnK:
				printf("Return\n");
				break;
			case DefineK:
				printf("Define: %s\n",tree->attr.name);
				break;
			case DefineAssignK:
				printf("Define Assign: %s\n",tree->attr.name);
				break;
			case NoHandK:
				printf("Hand\n");
				break;
			case NewArrayK:
				printf("New array: %s\n",tree->attr.name);
				break;
			case ArrayK:
				printf("Array: %s\n",tree->attr.name);
				break;
			case ArrayAssignK:
				printf("Assign to Array: %s\n",tree->attr.name);
				break;
			case ArrayNumK:
				printf("Array num\n");
				break;
			default:
				printf("unknown expnode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK) 
		{
			switch (tree->kind.exp) 
			{
			case OpK:
				printf("Op: ");
				printToken(tree->attr.op, "\0");
				break;
			case ConstK:
				printf("Const: %d\n", tree->attr.val);
				break;
			case IdK:
				printf("Id: %s\n", tree->attr.name);
				break;
			case StringK:
				printf("String: %s\n",tree->attr.name);
				break;
			default:
				printf("unknown expnode kind\n");
				break;
			}
		}
		else 
		{
			printf("unknown node kind\n");
		}
		for (i = 0; i < MAXCHILDREN; i++) 
		{
			_printTree(tree->child[i], indentno);
		}
		tree = tree->sibling;
	}
	indentno -= 2;
}

void printTree(TreeNode* tree) 
{
	_printTree(tree, 0);
}

static TreeNode* stmt_sequence(Parser* parser);
static TreeNode* statement(Parser* parser);
static TreeNode* if_stmt(Parser* parser);
static TreeNode* while_stmt(Parser* parser);
static TreeNode* dowhile_stmt(Parser* parser);
static TreeNode* assign_stmt(Parser* parser);
static TreeNode* read_stmt(Parser* parser);
static TreeNode* input_stmt(Parser* parser);
static TreeNode* write_stmt(Parser* parser);
static TreeNode* print_stmt(Parser* parser);
static TreeNode* include_stmt(Parser* parser);
static TreeNode* define_stmt(Parser* parser);
static TreeNode* newArray_stmt(Parser* parser);
static TreeNode* func_stmt(Parser* parser);
static TreeNode* params(Parser* parser);
static TreeNode* param(Parser* parser);
static TreeNode* call_stmt(Parser* parser);
static TreeNode* call_param(Parser* parser);
static TreeNode* return_stmt(Parser* parser);
static TreeNode* nohand_stmt(Parser* parser);
static TreeNode* exp(Parser* parser);
static TreeNode* exp_and(Parser* parser);
static TreeNode* exp_ass(Parser* parser);
static TreeNode* simple_exp(Parser* parser);
static TreeNode* term(Parser* parser);
static TreeNode* factor(Parser* parser);
TreeNode* compiler(char* filename);

static void syntaxError(Parser* parser,char* message) 
{
	printf("\n>>> ");
	printf("%s : Syntax error at line %d : %s", parser->lexer->filename,parser->lexer->lineno, message);
	parser->error = TRUE;
}

static void advance(Parser* parser) 
{
	parser->lastToken = parser->token;
	parser->token = lexer_getToken(parser->lexer);
}

static void match(Parser* parser, TokenType expected) 
{
	if (parser->token == expected) 
	{
		advance(parser);
	}
	else 
	{
		syntaxError(parser, (char*)"unexpected token -> ");
		printToken(parser->token, parser->lexer->tokenString);
		printf("           ");
	}
}

TreeNode* stmt_sequence(Parser* parser) 
{
	TreeNode* t = statement(parser);
	TreeNode* p = t;
	while (parser->token != ENDFILE) 
	{
		TreeNode* q;
		if (parser->lastToken != RBRACE && p->kind.stmt!=IncludeK && p->kind.stmt!=DefineK) 
		{
			match(parser, SEMI);
		}
		if (parser->token == RBRACE || parser->token == ENDFILE) 
		{
			break;
		}
		q = statement(parser);
		if (q != NULL) 
		{
			if (t == NULL) 
			{
				t = p = q;
			}
			else 
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

TreeNode* statement(Parser* parser)
{
	TreeNode* t = NULL;
	switch (parser->token) 
	{
	case IF:
		t = if_stmt(parser);
		break;
	case WHILE:
		t = while_stmt(parser);
		break;
	case DO:
		t = dowhile_stmt(parser);
		break;
	case ID:
		t = assign_stmt(parser);
		break;
	case INPUT:
		t = input_stmt(parser);
		break;
	case PRINT:
		t = print_stmt(parser);
		break;
	case INCLUDE:
		t=include_stmt(parser);
		break;
	case FUNC:
		t=func_stmt(parser);
		break;
	case RETURN:
		t=return_stmt(parser);
		break;
	case DEFINE:
		t=define_stmt(parser);
		define_list d;
		d.name=t->attr.name;
		d.val=t->child[0]->attr.name;
		definel.push_back(d);
		break;
	case LBRACE:
		t=nohand_stmt(parser);
		break;
	case ARRAY:
		t=newArray_stmt(parser);
		break;
	default:
		syntaxError(parser, (char*)"unexpected token -> ");
		printToken(parser->token, parser->lexer->tokenString);
		advance(parser);
		break;
	}
	return t;
}

TreeNode* if_stmt(Parser* parser) 
{
	TreeNode* t = newStmtNode(IfK, parser->lexer->lineno);
	match(parser,IF);
	match(parser, LPARAM);
	if (t != NULL) 
	{
		t->child[0] = exp(parser);
	}
	match(parser, RPARAM);
	match(parser, LBRACE);
	if (t != NULL) 
	{
		t->child[1] = stmt_sequence(parser);
	}
	match(parser, RBRACE);
	if (parser->token == ELSE) 
	{
		TreeNode* p = newStmtNode(ElseK, parser->lexer->lineno);
		match(parser, ELSE);
		match(parser, LBRACE);
		if (p != NULL)
		{
			p->child[0] = stmt_sequence(parser);
		}
		t->child[2]=p;
		match(parser, RBRACE);
	}
	return t;
}

static TreeNode* while_stmt(Parser* parser)
{
	TreeNode* t = newStmtNode(WhileK, parser->lexer->lineno);
	match(parser, WHILE);
	match(parser, LPARAM);
	if (t != NULL) 
	{
		t->child[0] = exp(parser);
	}
	match(parser, RPARAM);
	match(parser, LBRACE);
	if (t != NULL) 
	{
		t->child[1] = stmt_sequence(parser);
	}
	match(parser, RBRACE);
	return t;
}

static TreeNode* dowhile_stmt(Parser* parser)
{
	TreeNode* t = newStmtNode(DoWhileK, parser->lexer->lineno);
	match(parser, DO);
	match(parser, LBRACE);
	if (t != NULL) 
	{
		t->child[0] = stmt_sequence(parser);
	}
	match(parser, RBRACE);
	match(parser, WHILE);
	match(parser, LPARAM);
	if (t != NULL) 
	{
		t->child[1] = exp(parser);
	}
	match(parser, RPARAM);
	return t;
}

static TreeNode* func_stmt(Parser* parser)
{
	TreeNode* t=newStmtNode(FuncK,parser->lexer->lineno);
	match(parser,FUNC);
	if(t!=NULL&&parser->token==ID)
	{
		t->attr.name=copyString(parser->lexer->tokenString);
	}
	match(parser,ID);
	match(parser,LPARAM);
	if(t!=NULL&&parser->token!=RPARAM)
	{
		t->child[0]=params(parser);
	}
	match(parser,RPARAM);
	match(parser,LBRACE);
	if(t!=NULL&&parser->token!=RBRACE)
	{
		t->child[1]=stmt_sequence(parser);
	}
	match(parser,RBRACE);
	return t;
}

static TreeNode* params(Parser* parser)
{
	TreeNode* t=param(parser);
	TreeNode* p=t;
	match(parser,ID);
	while(parser->token==SIM)
	{
		TreeNode* q=NULL;
		match(parser,SIM);
		q=param(parser);
		match(parser,ID);
		if(q!=NULL)
		{
			if(t==NULL)
			{
				t=p=q;
			}
			else
			{
				p->sibling=q;
				p=q;
			}
		}
	}
	return t;
}

static TreeNode* param(Parser* parser)
{
	TreeNode* t=newStmtNode(ParamK,parser->lexer->lineno);
	if(t!=NULL)
	{
		t->attr.name=copyString(parser->lexer->tokenString);
	}
	return t;
}

TreeNode* assign_stmt(Parser* parser) 
{
	TreeNode* t = newStmtNode(AssignK, parser->lexer->lineno);
	if (t != NULL && parser->token == ID) 
	{
		t->attr.name = copyString(parser->lexer->tokenString);
	}
	match(parser, ID);
	if (t != NULL&&parser->token==ASSIGN) 
	{
		match(parser, ASSIGN);
		t->child[0] = exp(parser);
	}
	else if(t!=NULL&&parser->token==LBRACKET)
	{
		TreeNode* p=newStmtNode(ArrayAssignK,parser->lexer->lineno);
		p->attr.name=t->attr.name;
		t=p;
		match(parser,LBRACKET);
		p=newStmtNode(ArrayNumK,parser->lexer->lineno);
		p->child[0]=exp(parser);
		t->child[0]=p;
		match(parser,RBRACKET);
		match(parser, ASSIGN);
		t->child[1]=exp(parser);
	}
	else if(t!=NULL&&parser->token==LPARAM)
	{
		TreeNode* p = newStmtNode(CallK, parser->lexer->lineno);
		p->attr.name=t->attr.name;
		t=p;
		match(parser,LPARAM);
		if(t!=NULL&&parser->token!=RPARAM)
		{
			t->child[0]=call_stmt(parser);
		}
		match(parser,RPARAM);
	}
	return t;
}

static TreeNode* newArray_stmt(Parser* parser)
{
	TreeNode* t=newStmtNode(NewArrayK,parser->lexer->lineno);
	match(parser,ARRAY);
	if(t!=NULL)
	{
		t->attr.name=copyString(parser->lexer->tokenString);
	}
	match(parser,ID);
	match(parser,LBRACKET);
	if(t!=NULL)
	{
		TreeNode* p=newStmtNode(ArrayNumK,parser->lexer->lineno);
		p->child[0]=exp(parser);
		t->child[0]=p;
	}
	match(parser,RBRACKET);
	return t;
}

TreeNode* call_stmt(Parser* parser)
{
	TreeNode* t=call_param(parser);
	TreeNode* p=t;
	while(parser->token==SIM)
	{
		TreeNode* q=NULL;
		match(parser,SIM);
		q=call_param(parser);
		if(q!=NULL)
		{
			if(t==NULL)
			{
				t=p=q;
			}
			else
			{
				p->sibling=q;
				p=q;
			}
		}
	}
	return t;
}

TreeNode* call_param(Parser* parser)
{
	TreeNode* t = newStmtNode(CallParamK, parser->lexer->lineno);
	t->child[0]=exp(parser);
	return t;
}

TreeNode* read_stmt(Parser* parser) 
{
	TreeNode* t = newStmtNode(ReadK, parser->lexer->lineno);
	if (t != NULL && parser->token == ID) 
	{
		t->attr.name = copyString(parser->lexer->tokenString);
	}
	return t;
}

static TreeNode* input_stmt(Parser* parser)
{
	TreeNode* tt=newStmtNode(InputK,parser->lexer->lineno);
	match(parser,INPUT);
	match(parser,LPARAM);
	if(parser->token!=RPARAM)
	{
		TreeNode* t=read_stmt(parser);
		TreeNode* p=t;
		match(parser,ID);
		while(parser->token==SIM)
		{
			TreeNode* q=NULL;
			match(parser,SIM);
			q=read_stmt(parser);
			match(parser,ID);
			if(q!=NULL)
			{
				if(t==NULL)
				{
					t=p=q;
				}
				else
				{
					p->sibling=q;
					p=q;
				}
			}
		}
		tt->child[0]=t;
	}
	match(parser,RPARAM);
	return tt;
}

TreeNode* write_stmt(Parser* parser) 
{
	TreeNode* t = newStmtNode(WriteK, parser->lexer->lineno);
	if (t != NULL) 
	{
		t->child[0] = exp(parser);
	}
	return t;
}

static TreeNode* print_stmt(Parser* parser)
{
	TreeNode* tt=newStmtNode(PrintK,parser->lexer->lineno);
	match(parser,PRINT);
	match(parser,LPARAM);
	if(parser->token!=RPARAM)
	{
		TreeNode* t=write_stmt(parser);
		TreeNode* p=t;
		while(parser->token==SIM)
		{
			TreeNode* q=NULL;
			match(parser,SIM);
			q=write_stmt(parser);
			if(q!=NULL)
			{
				if(t==NULL)
				{
					t=p=q;
				}
				else
				{
					p->sibling=q;
					p=q;
				}
			}
		}
		tt->child[0]=t;
	}
	match(parser,RPARAM);
	return tt;
}

TreeNode* return_stmt(Parser* parser) 
{
	TreeNode* t = newStmtNode(ReturnK, parser->lexer->lineno);
	match(parser, RETURN);
	if (t != NULL) 
	{
		t->child[0] = exp(parser);
	}
	return t;
}

static TreeNode* nohand_stmt(Parser* parser)
{
	TreeNode* t=newStmtNode(NoHandK,parser->lexer->lineno);
	match(parser,LBRACE);
	if(parser->token!=RBRACE&&t!=NULL)
	{
		t->child[0]=stmt_sequence(parser);
	}
	match(parser,RBRACE);
	return t;
}

static TreeNode* include_stmt(Parser* parser)
{
	TreeNode* t=newStmtNode(IncludeK, parser->lexer->lineno);
	match(parser,INCLUDE);
	if(parser->token==STRING)
	{
		t->attr.name = copyString(parser->lexer->tokenString);
		t->child[0]=compiler(t->attr.name);
		match(parser,STRING);
	}
	else
	{
		match(parser,LT);
		t->attr.name = copyString(parser->lexer->tokenString);
		char buf[1024];
		sprintf(buf,"E:/王永健/王永健的文件夹/信息学/程序/随意/其他/c--1.0.0.1/include/%s.c--",t->attr.name);
		t->child[0]=compiler(buf);
		match(parser,ID);
		match(parser,GT);
	}
	return t;
}

static TreeNode* define_stmt(Parser* parser)
{
	TreeNode* t=newStmtNode(DefineK,parser->lexer->lineno);
	match(parser,DEFINE);
	if(t!=NULL)
	{
		t->attr.name=copyString(parser->lexer->tokenString);
	}
	int tokenStringlen=0;
	while(parser->lexer->linepos<parser->lexer->bufsize)
	{
		if(parser->lexer->lineBuf[parser->lexer->linepos]!=' '&&parser->lexer->lineBuf[parser->lexer->linepos]!='\t')
		{
			parser->lexer->tokenString[tokenStringlen++]=parser->lexer->lineBuf[parser->lexer->linepos];
		}
		parser->lexer->linepos++;
	}
	parser->lexer->tokenString[tokenStringlen-1]='\0';
	TreeNode* p=newStmtNode(DefineAssignK,parser->lexer->lineno);
	if(p!=NULL)
	{
		p->attr.name=copyString(parser->lexer->tokenString);
		match(parser,parser->token);
	}
	t->child[0]=p;
	return t;
}

static TreeNode* exp(Parser* parser)
{
	TreeNode* t = exp_and(parser);
	if (parser->token == OR) 
	{
		TreeNode* p = newExpNode(OpK, parser->lexer->lineno);
		if (p != NULL) 
		{
			p->child[0] = t;
			p->attr.op = parser->token;
			t = p;
		}
		match(parser, parser->token);
		if (t != NULL) 
		{
			t->child[1] = exp_and(parser);
		}
	}
	return t;
}

static TreeNode* exp_and(Parser* parser)
{
	TreeNode* t = exp_ass(parser);
	if (parser->token == AND) 
	{
		TreeNode* p = newExpNode(OpK, parser->lexer->lineno);
		if (p != NULL) 
		{
			p->child[0] = t;
			p->attr.op = parser->token;
			t = p;
		}
		match(parser, parser->token);
		if (t != NULL) 
		{
			t->child[1] = exp_ass(parser);
		}
	}
	return t;
}

TreeNode* exp_ass(Parser* parser) 
{
	TreeNode* t = simple_exp(parser);
	if (parser->token == LT || parser->token == EQ || parser->token == GT ||parser->token==GTD||parser->token==LTD||parser->token==NOTD) 
	{
		TreeNode* p = newExpNode(OpK, parser->lexer->lineno);
		if (p != NULL) 
		{
			p->child[0] = t;
			p->attr.op = parser->token;
			t = p;
		}
		match(parser, parser->token);
		if (t != NULL) 
		{
			t->child[1] = simple_exp(parser);
		}
	}
	return t;
}

TreeNode* simple_exp(Parser* parser) 
{
	TreeNode* t = term(parser);
	while (parser->token == PLUS || parser->token == MINUS) 
	{
		TreeNode* p = newExpNode(OpK, parser->lexer->lineno);
		if (p != NULL) 
		{
			p->child[0] = t;
			p->attr.op = parser->token;
			t = p;
			match(parser, parser->token);
			t->child[1] = term(parser);
		}
	}
	return t;
}

TreeNode* term(Parser* parser) 
{
	TreeNode* t = factor(parser);
	while (parser->token == TIMES || parser->token == OVER || parser->token == YTIMES) 
	{
		TreeNode* p = newExpNode(OpK, parser->lexer->lineno);
		if (p != NULL) 
		{
			p->child[0] = t;
			p->attr.op = parser->token;
			t = p;
			match(parser, parser->token);
			p->child[1] = factor(parser);
		}
	}
	return t;
}

TreeNode* factor(Parser* parser) 
{
	TreeNode* t = NULL;
	switch (parser->token) 
	{
	case NUM:
		t = newExpNode(ConstK, parser->lexer->lineno);
		if (t != NULL && parser->token == NUM) 
		{
			t->attr.val = atoi(parser->lexer->tokenString);
		}
		match(parser,NUM);
		break;
	case ID:
		t = newExpNode(IdK, parser->lexer->lineno);
		if (t != NULL && parser->token == ID) 
		{
			t->attr.name = copyString(parser->lexer->tokenString);
		}
		match(parser, ID);
		if(t!=NULL&&parser->token==LBRACKET)
		{
			TreeNode* p = newStmtNode(ArrayK,parser->lexer->lineno);
			p->attr.name=t->attr.name;
			t=p;
			match(parser,LBRACKET);
			p=newStmtNode(ArrayNumK,parser->lexer->lineno);
			p->child[0]=exp(parser);
			t->child[0]=p;
			match(parser,RBRACKET);
		}
		else if(t!=NULL&&parser->token==LPARAM)
		{
			TreeNode* p = newStmtNode(CallK, parser->lexer->lineno);
			p->attr.name=t->attr.name;
			t=p;
			match(parser, LPARAM);
			if(t!=NULL&&parser->token!=RPARAM)
			{
				t->child[0]=call_stmt(parser);
			}
			match(parser,RPARAM);
		}
		break;
	case STRING:
		t = newExpNode(StringK, parser->lexer->lineno);
		if (t != NULL && parser->token == STRING) 
		{
			t->attr.name = copyString(parser->lexer->tokenString);
		}
		match(parser, STRING);
		break;
	case LPARAM:
		match(parser, LPARAM);
		t = exp(parser);
		match(parser, RPARAM);
		break;
	default:
		syntaxError(parser,(char*)"unexpected token -> ");
		printToken(parser->token, parser->lexer->tokenString);
		advance(parser);
		break;
	}
	return t;
}

TreeNode* parser_parse(Parser* parser) 
{
	TreeNode* t;
	advance(parser);
	t = stmt_sequence(parser);
	if (parser->token != ENDFILE) 
	{
		syntaxError(parser,(char*)"code ends before file");
	}
	return t;
}

void st_init(Symtab* symtab) 
{
	if (symtab == NULL) 
	{
		return;
	}
	for (int i = 0; i < SIZE; i++) 
	{
		symtab->hashTable[i] = NULL;
	}
}

static int hash(char* key) 
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0') 
	{
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

void st_insert(Symtab* symtab, char* name, int lineno, int loc) 
{
	int h = hash(name);
	BucketList l = symtab->hashTable[h];
	while (l != NULL && strcmp(name, l->name) != 0) 
	{
		l = l->next;
	}
	if (l == NULL) 
	{
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->lines->next = NULL;
		l->next = symtab->hashTable[h];
		symtab->hashTable[h] = l;
	}
	else 
	{
		LineList t = l->lines;
		while (t->next != NULL) 
		{
			t = t->next;
		}
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
}
int st_lookup(Symtab* symtab, char* name) 
{
	int h = hash(name);
	BucketList l = symtab->hashTable[h];
	while (l != NULL && strcmp(name, l->name) != 0) 
	{
		l = l->next;
	}
	if (l == NULL) 
	{
		return -1;
	}
	else 
	{
		return l->memloc;
	}
}

void printSymTab(Symtab* symtab) 
{
	int i;
	printf("Variable Name  Location  Line Numbers\n\n");
	for (i = 0; i < SIZE; ++i) 
	{
		if (symtab->hashTable[i] != NULL) 
		{
			BucketList l = symtab->hashTable[i];
			while (l != NULL) 
			{
				LineList t = l->lines;
				printf("%-14s ", l->name);
				printf("%-8d  ", l->memloc);
				while (t != NULL) 
				{
					printf("%4d ", t->lineno);
					t = t->next;
				}
				printf("\n");
				l = l->next;
			}
		}
	}
}

void analyzer_init(Analyzer* analyzer, Symtab* symtab) 
{
	analyzer->symtab = symtab;
	analyzer->location = 0;
	analyzer->error = FALSE;
}

static void traverse(Analyzer* analyzer,TreeNode* t, void(*preProc)(Analyzer*,TreeNode*), void(*postProc)(Analyzer*,TreeNode*)) 
{
	if (t != NULL) 
	{
		preProc(analyzer,t);
		for (int i = 0; i < MAXCHILDREN; i++) 
		{
			traverse(analyzer,t->child[i], preProc, postProc);
		}
		postProc(analyzer,t);
		traverse(analyzer,t->sibling, preProc, postProc);
	}
}

static void nullProc(Analyzer* analyzer, TreeNode* t) 
{
	//if (t == NULL) {
	//	return;
	//}
	//else {
	//	return;
	//}
	return;
}

static void insertNode(Analyzer* analyzer,TreeNode* t) 
{
	switch (t->nodekind) 
	{
	case StmtK:
		switch (t->kind.stmt) 
		{
		case CallK:
		case FuncK:
		case AssignK:
		case ReadK:
			if (st_lookup(analyzer->symtab, t->attr.name) == -1) 
			{
				st_insert(analyzer->symtab,t->attr.name, t->lineno, analyzer->location++);
			}
			else 
			{
				st_insert(analyzer->symtab, t->attr.name, t->lineno, 0);
			}
			break;
		default:
			break;
		}
		break;
	case ExpK:
		switch (t->kind.exp) 
		{
		case IdK:
			if (st_lookup(analyzer->symtab, t->attr.name) == -1) 
			{
				st_insert(analyzer->symtab, t->attr.name, t->lineno, analyzer->location++);
			}
			else 
			{
				st_insert(analyzer->symtab, t->attr.name, t->lineno, 0);
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void buildSymtab(Analyzer* analyzer, TreeNode* syntaxTree) 
{
	traverse(analyzer,syntaxTree, insertNode, nullProc);
	printf("\nSymbol table:\n\n");
	printSymTab(analyzer->symtab);
}


static void typeError(Analyzer* analyzer, TreeNode* t, char* message) 
{
	printf("Type error at line %d: %s\n", t->lineno, message);
	analyzer->error = TRUE;
}

static void checkNode(Analyzer* analyzer, TreeNode* t) 
{
	switch (t->nodekind) 
	{
	case ExpK:
		switch (t->kind.exp) 
		{
		case OpK:
			if ((t->child[0]->type == Void || t->child[1]->type == Void)&&t->child[0]->nodekind!=StmtK) 
			{
				typeError(analyzer, t,(char*) "Op applied to non-integer");
			}
			if (t->attr.op == EQ || t->attr.op == LT || t->attr.op == GT||t->attr.op == LTD||t->attr.op == GTD||t->attr.op == NOTD||t->attr.op == OR||t->attr.op == AND) 
			{
				t->type = Boolean;
			}
			else 
			{
				t->type = Integer;
			}
			break;
		case ConstK:
		case StringK:
		case IdK:
			t->type = Integer;
			break;
		default:
			break;
		}
		break;
	case StmtK:
		switch (t->kind.stmt) 
		{
		case IfK:
			if (t->child[0]->type == Integer) 
			{
				typeError(analyzer,t->child[0], (char*)"if test is not Boolean");
			}
			break;
		case AssignK:
			if (t->child[0]->type != Integer&&t->child[0]->nodekind!=StmtK) 
			{
				typeError(analyzer, t->child[0], (char*)"assignment of non-integer value");
			}
			break;
		case WriteK:
			if (t->child[0]->type != Integer&&t->child[0]->nodekind!=StmtK) 
			{
				typeError(analyzer, t->child[0], (char*)"write of non-integer value");
			}
			break;
		case WhileK:
			if (t->child[0]->type == Integer) 
			{
				typeError(analyzer, t->child[0], (char*)"while test is not Boolean");
			}
			break;
		case DoWhileK:
			if (t->child[1]->type == Integer) 
			{
				typeError(analyzer, t->child[1], (char*)"do-while test is not Boolean");
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}


void typeCheck(Analyzer* analyzer, TreeNode* syntaxTree) 
{
	traverse(analyzer, syntaxTree, nullProc, checkNode);
}

void coder_init(Coder* coder,FILE* out, Symtab* symtab,int traceCode);
void emitRO(Coder* coder,char* op, int r, int s, int t, char* c);
void emitRM(Coder* coder, char* op, int r, int d, int s, char* c);
int emitSkip(Coder* coder, int howMany);
void emitBackup(Coder* coder, int loc);
void emitRestore(Coder* coder);
void emitComment(Coder* coder, char* c);
void emitRM_Abs(Coder* coder, char* op, int r, int a, char* c);

void coder_init(Coder* coder, FILE* out, Symtab* symtab, int traceCode) 
{
	coder->emitLoc = 0;
	coder->highEmitLoc = 0;
	coder->out = out;
	coder->traceCode = traceCode;
	coder->tmpOffset = 0;
	coder->symtab = symtab;
	coder->params=0;
	coder->arrays=0;
}

void emitRO(Coder* coder, char* op, int r, int s, int t, char* c) 
{
	fprintf(coder->out, "%3d:  %5s  %d,%d,%d", coder->emitLoc++, op, r, s, t);
	if (coder->traceCode) 
	{
		//fprintf(coder->out, "\t%s", c);
	}
	fprintf(coder->out, "\n");
	if (coder->highEmitLoc < coder->emitLoc) 
	{
		coder->highEmitLoc = coder->emitLoc;
	}
}

void emitRM(Coder* coder, char* op, int r, int d, int s, char* c) 
{
	fprintf(coder->out, "%3d:  %5s  %d,%d(%d)", coder->emitLoc++, op, r, d, s);
	if (coder->traceCode) 
	{
		//fprintf(coder->out, "\t%s", c);
	}
	fprintf(coder->out, "\n");
	if (coder->highEmitLoc < coder->emitLoc) 
	{
		coder->highEmitLoc = coder->emitLoc;
	}
}

int emitSkip(Coder* coder, int howMany) 
{
	int i = coder->emitLoc;
	coder->emitLoc += howMany;
	if (coder->highEmitLoc < coder->emitLoc) 
	{
		coder->highEmitLoc = coder->emitLoc;
	}
	return i;
}

void emitBackup(Coder* coder, int loc) 
{
	if (loc > coder->highEmitLoc) 
	{
		emitComment(coder,(char*)"BUG in emitBackup");
	}
	coder->emitLoc = loc;
}

void emitRestore(Coder* coder) 
{
	coder->emitLoc = coder->highEmitLoc;
}

void emitComment(Coder* coder, char* c) 
{
	if (coder->traceCode) 
	{
		//fprintf(coder->out, "* %s\n", c);
	}
}

void emitRM_Abs(Coder* coder, char* op, int r, int a, char* c) 
{
	fprintf(coder->out, "%3d:  %5s  %d,%d(%d)", coder->emitLoc, op, r, a - (coder->emitLoc + 1), pc);
	coder->emitLoc++;
	if (coder->traceCode) 
	{
		//fprintf(coder->out, "\t%s", c);
	}
	fprintf(coder->out, "\n");
	if (coder->highEmitLoc < coder->emitLoc) 
	{
		coder->highEmitLoc = coder->emitLoc;
	}
}

static void cGen(Coder* coder, TreeNode* tree);

int param_lookup(Coder* coder,char* name)
{
	int i=coder->param.size()-1;
	while(i>=0)
	{
		if(!strcmp(name, coder->param[i].name))
		{
			return coder->param[i].params;
		}
		i-=1;
	}
	return -1;
}

int func_lookup(Coder* coder,char* name)
{
	int i=0;
	while(i<coder->func.size())
	{
		if(!strcmp(name, coder->func[i].name))
		{
			return i;
		}
		i+=1;
	}
	return -1;
}

int call_lookup(Coder* coder,char* name)
{
	int i=0;
	while(i<coder->call.size())
	{
		if(!strcmp(name,coder->call[i].name))
		{
			return i;
		}
		i+=1;
	}
	return -1;
}

int array_lookup(Coder* coder,char* name)
{
	int i=0;
	while(i<coder->array.size())
	{
		if(!strcmp(name,coder->array[i].name))
		{
			return coder->array[i].params;
		}
		i+=1;
	}
	return -1;
}

static void genStmt(Coder* coder, TreeNode* tree) 
{
	TreeNode *p1, *p2, *p3;
	int savedLoc1, savedLoc2, currentLoc;
	int loc;
	int a;
	switch (tree->kind.stmt) 
	{
	case IfK:
		if (coder->traceCode) 
		{
			emitComment(coder, (char*)"-> if");
		}
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];
		/* generate code for test expression */
		cGen(coder, p1);
		savedLoc1 = emitSkip(coder, 1);
		emitComment(coder, (char*)"if: jump to else belongs here");
		/* recurse on then part */
		cGen(coder, p2);
		savedLoc2 = emitSkip(coder, 1);
		emitComment(coder, (char*)"if: jump to end belongs here");
		currentLoc = emitSkip(coder, 0);
		emitBackup(coder, savedLoc1);
		emitRM_Abs(coder, (char*)"JEQ", ac, currentLoc, (char*)"if: jmp to else");
		emitRestore(coder);
		/* recurse on else part */
		cGen(coder,p3);
		currentLoc = emitSkip(coder, 0);
		emitBackup(coder, savedLoc2);
		emitRM_Abs(coder, (char*)"LDA", pc, currentLoc, (char*)"jmp to end");
		emitRestore(coder);
		if (coder->traceCode)  emitComment(coder, (char*)"<- if");
		break; /* if_k */
	case WhileK:
		if(coder->traceCode) emitComment(coder,(char*) "-> while");
		p1 = tree->child[0];
		p2 = tree->child[1];
		savedLoc1 = emitSkip(coder, 0);
		cGen(coder, p1);
		savedLoc2 = emitSkip(coder, 1);
		emitComment(coder,(char*) "while: jump to end belongs here");
		cGen(coder, p2);
		currentLoc = emitSkip(coder, 0);
		emitRM_Abs(coder, (char*)"LDA", pc, savedLoc1, (char*)"jmp back to loop");
		emitBackup(coder,savedLoc2);
		emitRM_Abs(coder,(char*)"JEQ", ac, currentLoc+1, (char*)"while: jmp to end");
		emitRestore(coder);
		if (coder->traceCode)  emitComment(coder, (char*)"<- while");
		break; /* while_k */
	case DoWhileK:
		if(coder->traceCode) emitComment(coder,(char*) "-> dowhile");
		p1 = tree->child[0];
		p2 = tree->child[1];
		savedLoc1 = emitSkip(coder, 0);
		cGen(coder, p1);
		cGen(coder, p2);
		emitComment(coder,(char*) "dowhile: jump to end belongs here");
		emitRM(coder,(char*) "JEQ", ac, 1, pc,(char*) "dowhile: jmp to end");
		emitRM_Abs(coder, (char*)"LDA", pc, savedLoc1, (char*)"jmp back to loop");
		emitRestore(coder);
		if (coder->traceCode)  emitComment(coder, (char*)"<- dowhile");
		break;
	case AssignK:
		if (coder->traceCode) emitComment(coder, (char*)"-> assign");
		/* generate code for rhs */
		cGen(coder, tree->child[0]);
		/* now store value */
		loc = param_lookup(coder, tree->attr.name);
		if(loc!=-1)
		{
			emitRM(coder,(char*)"ASTP",ac,loc,gp,(char*)"assign: store value");
		}
		else
		{
			loc=st_lookup(coder->symtab,tree->attr.name);
			emitRM(coder, (char*)"ST", ac, loc, gp, (char*)"assign: store value");
		}
		if (coder->traceCode)  emitComment(coder, (char*)"<- assign");
		break; /* assign_k */
	case ElseK:
		if(coder->traceCode) emitComment(coder,(char*) "-> else");
		cGen(coder,tree->child[0]);
		if(coder->traceCode) emitComment(coder,(char*) "<- else");
		break;

	case ReadK:
		emitRO(coder, (char*)"IN", ac, 0, 0,(char*) "read integer value");
		loc = st_lookup(coder->symtab, tree->attr.name);
		emitRM(coder, (char*)"ST", ac, loc, gp,(char*) "read: store value");
		break;
	case InputK:
		if(coder->traceCode) emitComment(coder,(char*) "-> input");
		cGen(coder,tree->child[0]);
		if(coder->traceCode) emitComment(coder,(char*) "<- input");
		break;
	case WriteK:
		/* generate code for expression to write */
		cGen(coder, tree->child[0]);
		/* now output it */
		emitRO(coder, (char*)"OUT", ac, 0, 0,(char*) "write ac");
		break;
	case PrintK:
		if(coder->traceCode) emitComment(coder,(char*) "-> print");
		cGen(coder,tree->child[0]);
		if(coder->traceCode) emitComment(coder,(char*) "<- print");
		break;
	case IncludeK:
		if(coder->traceCode) emitComment(coder,(char*) "-> include");
		cGen(coder, tree->child[0]);
		if(coder->traceCode) emitComment(coder,(char*) "<- include");
		break;
	case ReturnK:
		if(coder->traceCode) emitComment(coder,(char*) "-> return");
		cGen(coder,tree->child[0]);
		a=func_lookup(coder,coder->nowfunc);
		emitRM(coder,(char*)"RET",pc,coder->func[a].params,0,(char*)"return");
		if(coder->traceCode) emitComment(coder,(char*) "<- return");
		break;
	case ParamK:
		if(coder->traceCode) emitComment(coder,(char*) "-> Param");
		cod p;
		p.name=tree->attr.name;
		p.params=coder->params;
		coder->param.push_back(p);
		coder->params+=1;
		if(coder->traceCode) emitComment(coder,(char*) "<- Param");
		break;
	case FuncK:
		if(coder->traceCode) emitComment(coder,(char*) "-> func");
		p1 = tree->child[0];
		p2 = tree->child[1];
		savedLoc1 = emitSkip(coder, 1);
		cGen(coder, p1);
		a=call_lookup(coder,tree->attr.name);
		if(a!=-1)
		{
			int currentLoc=coder->emitLoc;
            emitBackup(coder,coder->call[a].way);
            emitRM(coder,(char*)"CALL",pc,currentLoc,coder->params,(char*)"jmp to func");
            emitRestore(coder);
            coder->call.erase(coder->call.begin()+a);
		}
		if(func_lookup(coder,tree->attr.name)==-1)
		{
			cod f;
			f.name=tree->attr.name;
			f.way=savedLoc1;
			f.params=coder->params;
			coder->func.push_back(f);
			coder->params=0;
		}
		coder->nowfunc=tree->attr.name;
		savedLoc2 = emitSkip(coder, 0);
		cGen(coder, p2);
		a=func_lookup(coder,tree->attr.name);
		emitRM(coder,(char*)"RET",pc,coder->func[a].params,0,(char*)"return func");
		currentLoc = emitSkip(coder, 0);
		emitBackup(coder,savedLoc1);
		emitRM_Abs(coder, (char*)"LDA", pc, currentLoc, (char*)"jmp to end");
		emitRestore(coder);
		for(int i=0;i<coder->func[a].params;i++)
		{
			if(!coder->param.empty())
			{
				coder->param.pop_back();
			}
		}
		if (coder->traceCode)  emitComment(coder, (char*)"<- func");
		break; 
	case CallParamK:
		if(coder->traceCode) emitComment(coder,(char*) "-> CallParam");
		cGen(coder,tree->child[0]);
		emitRM(coder,(char*)"STP",ac,0,0,(char*)"store param");
		if(coder->traceCode) emitComment(coder,(char*) "<- CallParam");
		break;
	case CallK:
		if(coder->traceCode) emitComment(coder,(char*) "-> call");
		cGen(coder,tree->child[0]);
		currentLoc=func_lookup(coder,tree->attr.name);
		if(currentLoc==-1)
		{
			savedLoc1=emitSkip(coder,1);
			cod cl;
			cl.name=tree->attr.name;
			cl.way=savedLoc1;
			coder->call.push_back(cl);
		}
		else
		{
			emitRM(coder,(char*)"CALL",pc,coder->func[currentLoc].way+1,coder->func[currentLoc].params,(char*)"jmp to func");
		}
		emitRestore(coder);
		if(coder->traceCode) emitComment(coder,(char*) "<- call");
		break;
	case NoHandK:
		cGen(coder,tree->child[0]);
		break;
	case ArrayNumK:
		if(coder->traceCode) emitComment(coder,(char*) "-> arraynum");
		cGen(coder,tree->child[0]);
		emitRM(coder,(char*)"STNA",ac,0,0,(char*)"array num");
		if(coder->traceCode) emitComment(coder,(char*) "<- arraynum");
		break;
	case NewArrayK:
		if(coder->traceCode) emitComment(coder,(char*) "-> new array");
		cGen(coder,tree->child[0]);
		cod cd;
		cd.name=tree->attr.name;
		cd.params=coder->arrays;
		coder->arrays++;
		coder->array.push_back(cd);
		emitRM(coder,(char*)"NEWA",ac,0,0,(char*)"new array");
		if(coder->traceCode) emitComment(coder,(char*) "<- new array");
		break;
	case ArrayAssignK:
		if(coder->traceCode) emitComment(coder,(char*) "-> array assign");
		cGen(coder,tree->child[0]);
		cGen(coder,tree->child[1]);
		loc=array_lookup(coder,tree->attr.name);
		if(loc!=-1)
		{
			emitRM(coder,(char*)"STA",ac,loc,0,(char*)"store array");
		}
		if(coder->traceCode) emitComment(coder,(char*) "<- array assign");
		break;
	case ArrayK:
		if(coder->traceCode) emitComment(coder,(char*) "-> array");
		cGen(coder,tree->child[0]);
		loc=array_lookup(coder,tree->attr.name);
		if(loc!=-1)
		{
			emitRM(coder,(char*)"ALDA",ac,loc,0,(char*)"store array");
		}
		if(coder->traceCode) emitComment(coder,(char*) "<- array");
		break;
	default:
		break;
	}
}

/* Procedure genExp generates code at an expression node */
static void genExp(Coder* coder,TreeNode * tree)
{
	int loc;
	TreeNode * p1, *p2;
	int pop;
	switch (tree->kind.exp) 
	{

	case ConstK:
		if (coder->traceCode) emitComment(coder,(char*)"-> Const");
		/* gen code to load integer constant using LDC */
		emitRM(coder,(char*)"LDC", ac, tree->attr.val, 0, (char*)"load const");
		if (coder->traceCode)  emitComment(coder,(char*)"<- Const");
		break; /* ConstK */

	case IdK:
		if (coder->traceCode) emitComment(coder,(char*)"-> Id");
		loc = param_lookup(coder,tree->attr.name);
		if(loc!=-1)
		{
			emitRM(coder,(char*)"LDP",ac,loc,gp,(char*)"ld param");
		}
		else
		{
			loc=st_lookup(coder->symtab,tree->attr.name);
			emitRM(coder,(char*)"LD",ac,loc,gp,(char*)"load id value");
		}
		if (coder->traceCode)  emitComment(coder,(char*)"<- Id");
		break; /* IdK */

	case OpK:
		if (coder->traceCode) emitComment(coder,(char*)"-> Op");
		p1 = tree->child[0];
		p2 = tree->child[1];
		/* gen code for ac = left arg */
		cGen(coder,p1);
		/* gen code to push left operand */
		pop=coder->tmpOffset;
		emitRM(coder,(char*)"POP",ac,pop,mp,(char*)"pop");
		emitRM(coder,(char*)"ST", ac, coder->tmpOffset--, mp, (char*)"op: push left");
		/* gen code for ac = right operand */
		cGen(coder,p2);
		/* now load left operand */
		emitRM(coder,(char*)"LD", ac1, ++coder->tmpOffset, mp, (char*)"op: load left");
		switch (tree->attr.op) 
		{
		case PLUS:
			emitRO(coder,(char*)"ADD", ac, ac1, ac,(char*) "op +");
			break;
		case MINUS:
			emitRO(coder,(char*)"SUB", ac, ac1, ac, (char*)"op -");
			break;
		case TIMES:
			emitRO(coder,(char*)"MUL", ac, ac1, ac,(char*) "op *");
			break;
		case OVER:
			emitRO(coder,(char*)"DIV", ac, ac1, ac, (char*)"op /");
			break;
		case YTIMES:
			emitRO(coder,(char*)"RDE",ac,ac1,ac,(char*)"op %");
			break;
		case LT:
			emitRO(coder,(char*)"SUB", ac, ac1, ac, (char*)"op <");
			emitRM(coder,(char*)"JLT", ac, 2, pc, (char*)"br if true");
			emitRM(coder,(char*)"LDC", ac, 0, ac, (char*)"false case");
			emitRM(coder,(char*)"LDA", pc, 1, pc, (char*)"unconditional jmp");
			emitRM(coder,(char*)"LDC", ac, 1, ac, (char*)"true case");
			break;
		case GT:
			emitRO(coder, (char*)"SUB", ac, ac1, ac, (char*)"op >");
			emitRM(coder, (char*)"JGT", ac, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac, 0, ac,(char*) "false case");
			emitRM(coder,(char*) "LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac, 1, ac,(char*) "true case");
			break;
		case EQ:
			emitRO(coder,(char*) "SUB", ac, ac1, ac, (char*)"op ==");
			emitRM(coder,(char*) "JEQ", ac, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac, 0, ac, (char*)"false case");
			emitRM(coder, (char*)"LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac, 1, ac,(char*) "true case");
			break;
		case GTD:
			emitRO(coder, (char*)"SUB", ac, ac1, ac, (char*)"op >=");
			emitRM(coder, (char*)"JGE", ac, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac, 0, ac,(char*) "false case");
			emitRM(coder,(char*) "LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac, 1, ac,(char*) "true case");
			break;
		case LTD:
			emitRO(coder, (char*)"SUB", ac, ac1, ac, (char*)"op <=");
			emitRM(coder, (char*)"JLE", ac, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac, 0, ac,(char*) "false case");
			emitRM(coder,(char*) "LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac, 1, ac,(char*) "true case");
			break;
		case NOTD:
			emitRO(coder, (char*)"SUB", ac, ac1, ac, (char*)"op !=");
			emitRM(coder, (char*)"JNE", ac, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac, 0, ac,(char*) "false case");
			emitRM(coder,(char*) "LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac, 1, ac,(char*) "true case");
			break;
		case AND:
			emitRO(coder, (char*)"MUL", ac, ac1, ac, (char*)"op &&");
			emitRM(coder, (char*)"AND", ac, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac, 0, ac,(char*) "false case");
			emitRM(coder,(char*) "LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac, 1, ac,(char*) "true case");
			break;
		case OR:
			emitRM(coder,(char*)"LDC",ord , 0, 0, (char*)"load const");
			
			emitRO(coder, (char*)"SUB", orp, ac1, ord, (char*)"op !=");
			emitRM(coder, (char*)"JNE", orp, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac1, 0, ac1,(char*) "false case");
			emitRM(coder,(char*) "LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac1, 1, ac1,(char*) "true case");
			
			emitRO(coder, (char*)"SUB", orp, ac, ord, (char*)"op !=");
			emitRM(coder, (char*)"JNE", orp, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac, 0, ac,(char*) "false case");
			emitRM(coder,(char*) "LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac, 1, ac,(char*) "true case");
			
			emitRO(coder, (char*)"ADD", ac, ac1, ac, (char*)"op ||");
			emitRM(coder, (char*)"OR", ac, 2, pc,(char*) "br if true");
			emitRM(coder, (char*)"LDC", ac, 0, ac,(char*) "false case");
			emitRM(coder,(char*) "LDA", pc, 1, pc,(char*) "unconditional jmp");
			emitRM(coder,(char*) "LDC", ac, 1, ac,(char*) "true case");
			break;
		default:
			emitComment(coder, (char*)"BUG: Unknown operator");
			break;
		} /* case op */
		emitRM(coder,(char*)"BACK",ac,pop,mp,(char*)"push");
		if (coder->traceCode)  emitComment(coder,(char*) "<- Op");
		break; /* OpK */

	default:
		break;
	}
} /* genExp */

  /* Procedure cGen recursively generates code by
  * tree traversal
  */

static void cGen(Coder* coder,TreeNode * tree)
{
	if (tree != NULL)
	{
		switch (tree->nodekind) 
		{
		case StmtK:
			genStmt(coder,tree);
			break;
		case ExpK:
			genExp(coder,tree);
			break;
		default:
			break;
		}
		cGen(coder,tree->sibling);
	}
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
* file by traversal of the syntax tree. The
* second parameter (codefile) is the file name
* of the code file, and is used to print the
* file name as a comment in the code file
*/

void codeGen(Coder* coder, TreeNode* syntaxTree, char* codefile) 
{
	char * s = (char*)malloc(strlen(codefile) + 7);
	strcpy(s, "File: ");
	strcat(s, codefile);
	emitComment(coder,(char*)"TINY Compilation to TM Code");
	emitComment(coder, s);
	/* generate standard prelude */
	emitComment(coder, (char*)"Standard prelude:");
	emitRM(coder,(char*) "LD", mp, 0, ac, (char*)"load maxaddress from location 0");
	emitRM(coder,(char*) "ST", ac, 0, ac, (char*)"clear location 0");
	emitComment(coder,(char*) "End of standard prelude.");
	/* generate code for TINY program */
	cGen(coder, syntaxTree);
	
	int currentLoc=func_lookup(coder,(char*)"main");
    emitRM(coder,(char*)"CALL",pc,coder->func[currentLoc].way+1,coder->func[currentLoc].params,(char*)"jmp to func");
    emitRestore(coder);
	/* finish */
	emitComment(coder, (char*)"End of execution.");
	emitRO(coder,(char*) "HALT", 0, 0, 0, (char*)"");
}

TreeNode* compiler(char* filename)
{
	FILE* source = fopen(filename, "r");
	if (source == NULL) 
	{
		fprintf(stderr, "%s: source code not found",filename);
		exit(1);
	}
	Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
	if (lexer == NULL) 
	{
		exit(1);
	}
	lexer_init(lexer,source,filename);
	
	Parser* parser = (Parser*)malloc(sizeof(Parser));
	parser_init(parser, lexer);
	TreeNode* syntaxTree = parser_parse(parser);
	fclose(source);
	
	return syntaxTree;
}

int main(int argc, char * argv[]) 
{
	FILE* source = fopen("in.c--", "r");
	if (source == NULL) 
	{
		fprintf(stderr, "source code not found");
		exit(1);
	}
	Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
	if (lexer == NULL) 
	{
		exit(1);
	}
	lexer_init(lexer,source,(char*)"in.c--");
	
	Parser* parser = (Parser*)malloc(sizeof(Parser));
	parser_init(parser, lexer);
	TreeNode* syntaxTree = parser_parse(parser);
	printTree(syntaxTree);
	fclose(source);

	Symtab* symtab = (Symtab*)malloc(sizeof(Symtab));
	st_init(symtab);
	Analyzer* analyzer = (Analyzer*)malloc(sizeof(Analyzer));
	analyzer_init(analyzer, symtab);
	printf("\n Building Symbol Table... \n");
	buildSymtab(analyzer, syntaxTree);
	printf("\n Checking Types...\n");
	typeCheck(analyzer, syntaxTree);
	printf("\n Type Checking Finished");

	Coder* coder = (Coder*)malloc(sizeof(Coder));
	char* codefile = (char*)"in.c--exe";
	FILE* out = fopen(codefile, "w");
	if (out == NULL) 
	{
		exit(1);
	}
	coder_init(coder,out , symtab, TRUE);
	codeGen(coder, syntaxTree, codefile);
	fclose(out);

	free(parser);
	free(lexer);
	free(symtab);
	free(analyzer);
	free(coder);
	
	return 0;
}
