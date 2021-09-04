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
	FILE* out;
	Symtab* symtab;
	char* way;
	int realway;
	char* cway;
	int temp;
	int params;
	char* filename;
	int freturn;
}Coder;

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
		sprintf(buf,"E:/王永健/王永健的文件夹/信息学/程序/随意/其他/C--/include/%s.c--",t->attr.name);
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

void coder_init(Coder* coder,FILE* out,Symtab* symtab,char* filename)
{
	coder->out=out;
	coder->symtab=symtab;
	coder->way=(char*)"L1";
	coder->realway=1;
	coder->cway=(char*)"L1:";
	coder->temp=1024;
	coder->params=1024;
	coder->filename=filename;
	coder->freturn=1;
}

void opcode_one(Coder* coder,char* first)
{
	fprintf(coder->out,"%s\n",first);
}

void opcode_two(Coder* coder,char* first,char* second)
{
	fprintf(coder->out,"\t%s\t%s\n",first,second);
}

void opcode_three(Coder* coder,char* first,char* second,char* third)
{
	fprintf(coder->out,"\t%s\t%s,%s\n",first,second,third);
}

void way_add(Coder* coder)
{
	char buf[1024];
	char buff[1024];
	coder->realway++;
	sprintf(buf,"L%d",coder->realway);
	coder->way=buf;
	sprintf(buff,"L%d:",coder->realway);
	coder->cway=buff;
}

void way_sub(Coder* coder)
{
	char buf[1024];
	char buff[1024];
	coder->realway--;
	sprintf(buf,"L%d",coder->realway);
	coder->way=buf;
	sprintf(buff,"L%d:",coder->realway);
	coder->cway=buff;
}

void cGen(Coder* coder,TreeNode* tree);

void gen_stmt(Coder* coder,TreeNode* tree)
{
	char buf[1024];
	int loc;
	switch(tree->kind.stmt)
	{
		case IfK:
			cGen(coder,tree->child[0]);
			cGen(coder,tree->child[1]);
			way_add(coder);
			opcode_two(coder,(char*)"jmp",coder->way);
			way_sub(coder);
			opcode_one(coder,coder->cway);
			way_add(coder);
			cGen(coder,tree->child[2]);
			opcode_one(coder,coder->cway);
			way_add(coder);
			break;
		case ElseK:
			cGen(coder,tree->child[0]);
			break;
		case WhileK:
			opcode_one(coder,coder->cway);
			way_add(coder);
			cGen(coder,tree->child[0]);
			cGen(coder,tree->child[1]);
			way_sub(coder);
			opcode_two(coder,(char*)"jmp",coder->way);
			way_add(coder);
			opcode_one(coder,coder->cway);
			way_add(coder);
			break;
		case DoWhileK:
			opcode_one(coder,coder->cway);
			way_add(coder);
			cGen(coder,tree->child[0]);
			cGen(coder,tree->child[1]);
			way_sub(coder);
			opcode_two(coder,(char*)"jmp",coder->way);
			way_add(coder);
			opcode_one(coder,coder->cway);
			way_add(coder);
			break;
		case AssignK:
			cGen(coder,tree->child[0]);
			loc=st_lookup(coder->symtab,tree->attr.name);
			sprintf(buf,"dword ptr [%d]",loc);
			opcode_three(coder,(char*)"mov",buf,(char*)"%edx");
			break;
		case ReadK:
			loc=st_lookup(coder->symtab,tree->attr.name);
			opcode_three(coder,(char*)"mov",(char*)"ah",(char*)"01h");
			opcode_two(coder,(char*)"int",(char*)"21h");
			sprintf(buf,"dword ptr [%d]",loc);
			opcode_three(coder,(char*)"mov",buf,(char*)"al");
			break;
		case InputK:
			cGen(coder,tree->child[0]);
			break;
		case WriteK:
			cGen(coder,tree->child[0]);
			opcode_three(coder,(char*)"mov",(char*)"dl",(char*)"%edx");
			opcode_three(coder,(char*)"mov",(char*)"ah",(char*)"02h");
			opcode_two(coder,(char*)"int",(char*)"21h");
			break;
		case PrintK:
			cGen(coder,tree->child[0]);
			break;
		case ReturnK:
			cGen(coder,tree->child[0]);
			opcode_one(coder,(char*)"\tret");
			coder->freturn=0;
			break;
		case FuncK:
			sprintf(buf,"%s:",tree->attr.name);
			opcode_one(coder,buf);
			opcode_two(coder,(char*)"push",(char*)"%ebp");
			opcode_three(coder,(char*)"mov",(char*)"%ebp",(char*)"%esp");
			cGen(coder,tree->child[0]);
			cGen(coder,tree->child[1]);
			if(coder->freturn)
			{
				opcode_one(coder,(char*)"\tret");
			}
			coder->params=1024;
			break;
		case ParamK:
			sprintf(buf,"word ptr [%d]",coder->params--);
			opcode_two(coder,(char*)"push",(char*)"%esp");
			opcode_three(coder,(char*)"mov",buf,(char*)"%esp");
			break;
		case CallParamK:
			cGen(coder,tree->child[0]);
			opcode_three(coder,(char*)"mov",(char*)"%ecx",(char*)"%edx");
			opcode_two(coder,(char*)"push",(char*)"%ecx");
			break;
		case CallK:
			cGen(coder,tree->child[0]);
			opcode_two(coder,(char*)"call",tree->attr.name);
			break;
		case IncludeK:
			char buf[1024];
			sprintf(buf,"\"%s\"",tree->attr.name);
			opcode_two(coder,(char*)".file",buf);
			cGen(coder,tree->child[0]);
			break;
	}
}

void gen_exp(Coder* coder,TreeNode* tree)
{
	char buf[1024];
	int loc;
	switch(tree->kind.exp)
	{
		case ConstK:
			sprintf(buf,"$%d",tree->attr.val);
			opcode_three(coder,(char*)"mov",(char*)"%edx",buf);
			break;
		case IdK:
			loc=st_lookup(coder->symtab,tree->attr.name);
			sprintf(buf,"dword ptr [%d]",loc);
			opcode_three(coder,(char*)"mov",(char*)"%edx",buf);
			break;
		case StringK:
			break;
		case OpK:
			cGen(coder,tree->child[0]);
			sprintf(buf,"dword ptr [%d]",coder->temp--);
			opcode_three(coder,(char*)"mov",buf,(char*)"%edx");
			cGen(coder,tree->child[1]);
			sprintf(buf,"dword ptr [%d]",++coder->temp);
			opcode_three(coder,(char*)"mov",(char*)"%ebx",buf);
			switch(tree->attr.op)
			{
				case PLUS:
					opcode_three(coder,(char*)"add",(char*)"%edx",(char*)"%ebx");
					break;
				case MINUS:
					opcode_three(coder,(char*)"sub",(char*)"%edx",(char*)"%ebx");
					break;
				case TIMES:
					opcode_three(coder,(char*)"imul",(char*)"%edx",(char*)"%ebx");
					break;
				case OVER:
					opcode_three(coder,(char*)"idiv",(char*)"%edx",(char*)"%ebx");
					break;
				case EQ:
					opcode_three(coder,(char*)"cmp",(char*)"%edx",(char*)"%ebx");
					opcode_two(coder,(char*)"jeq",coder->way);
					break;
				case GT:
					opcode_three(coder,(char*)"cmp",(char*)"%edx",(char*)"%ebx");
					opcode_two(coder,(char*)"jgt",coder->way);
					break;
				case LT:
					opcode_three(coder,(char*)"cmp",(char*)"%edx",(char*)"%ebx");
					opcode_two(coder,(char*)"jlt",coder->way);
					break;
				case GTD:
					opcode_three(coder,(char*)"cmp",(char*)"%edx",(char*)"%ebx");
					opcode_two(coder,(char*)"jge",coder->way);
					break;
				case LTD:
					opcode_three(coder,(char*)"cmp",(char*)"%edx",(char*)"%ebx");
					opcode_two(coder,(char*)"jle",coder->way);
					break;
				case NOTD:
					opcode_three(coder,(char*)"cmp",(char*)"%edx",(char*)"%ebx");
					opcode_two(coder,(char*)"jne",coder->way);
					break;
				case AND:
					opcode_three(coder,(char*)"CMP",(char*)"%edx",(char*)"%ebx");
					opcode_two(coder,(char*)"and",coder->way);
					break;
				case OR:
					opcode_three(coder,(char*)"CMP",(char*)"%edx",(char*)"%ebx");
					opcode_two(coder,(char*)"or",coder->way);
					break;
			}
			break;
	}
}

void cGen(Coder* coder,TreeNode* tree)
{
	if(tree!=NULL)
	{
		switch(tree->nodekind)
		{
			case StmtK:
				gen_stmt(coder,tree);
				break;
			case ExpK:
				gen_exp(coder,tree);
				break;
		}
		cGen(coder,tree->sibling);
	}
}

void cgen_begin(Coder* coder,TreeNode* tree)
{
	char buf[1024];
	sprintf(buf,"\"%s\"",coder->filename);
	opcode_two(coder,(char*)".file",buf);
	cGen(coder,tree);
	opcode_two(coder,(char*)"call",(char*)"main");
}

TreeNode* compiler(char* filename)
{
	FILE* source = fopen(filename, "r");
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
	
	Coder* coder=(Coder*)malloc(sizeof(Coder));
	char* codefile = (char*)"in.s";
	FILE* out = fopen(codefile, "w");
	if (out == NULL) 
	{
		exit(1);
	}
	coder_init(coder,out,symtab,(char*)"in.c--");
	cgen_begin(coder,syntaxTree);
	fclose(out);

	free(parser);
	free(lexer);
	free(symtab);
	free(analyzer);
	free(coder);
	
	return 0;
}
