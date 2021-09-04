#include "header-file/compiler.h"

static char* file;
int linepos=0;
int lineposs=0;
int maxlen;
bool EOF_FILE=false;
TokenType token;
char tokenstring[MAXTOKENSTRING];
char ch;
int lineno;
char* File;

char getCh()
{
	if(lineposs<maxlen)
	{
		if(file[lineposs]=='\n')
		{
			lineno++;
			linepos=0;
		}
		linepos++;
		return file[lineposs++];
	}
	else
	{
		EOF_FILE=true;
		return EOF;
	}
}

void ungetCh()
{
	if(!EOF_FILE)
	{
		linepos--;
		lineposs--;
	}
}

int is_nodigit(char c)
{
	return (c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_';
}

int is_digit(char c)
{
	return c>='0'&&c<='9';
}

TokenType getId()
{
	int tokenstringlen=0;
	while(is_nodigit(ch)||is_digit(ch))
	{
		tokenstring[tokenstringlen++]=ch;
		ch=getCh();
	}
	tokenstring[tokenstringlen]='\0';
	for(int i=0;i<MAXTOKENSTR;i++)
	{
		if(!strcmp(tokenstring,tokenstr[i].str))
		{
			return tokenstr[i].tk;
		}
	}
	return TK_ID;
}

void getNum()
{
	int tokenstringlen=0;
	while(is_digit(ch))
	{
		tokenstring[tokenstringlen++]=ch;
		ch=getCh();
	}
	if(ch=='.')
	{
		ch=getCh();
		while(is_digit(ch))
		{
			tokenstring[tokenstringlen++]=ch;
			ch=getCh();
		}
	} 
	tokenstring[tokenstringlen]='\0';
}

void getString()
{
	int tokenstringlen=0;
	ch=getCh();
	while(ch!='"')
	{
		if(ch=='\\')
		{
			ch=getCh();
			switch(ch)
			{
				case 'n':
					ch='\n';
					break;
				case 't':
					ch='\t';
					break;
				case '0':
					ch='\0';
					break;
				case '\\':
					ch='\\';
					break;
				case '"':
					ch='"';
					break;
				default:
					error((char*)"非法转义字符",File,lineno,linepos);
			}
		}
		tokenstring[tokenstringlen++]=ch;
		ch=getCh();
	}
	tokenstring[tokenstringlen]='\0';
}

void gettoken()
{
	while(ch=='\n'||ch==' '||ch=='\t')
	{
		ch=getCh();
	}
	switch(ch)
	{
		case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':
		case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':
		case 'o':case 'p':case 'q':case 'r':case 's':case 't':
		case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':
		case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':
		case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':
		case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':
		case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':
		case '_':
			{
				token=getId();
			}
			break;
			
		case '0':case '1':case '2':case '3':
		case '4':case '5':case '6':case '7':
		case '8':case '9':
			getNum();
			token=TK_NUM;
			break;
			
		case '+':
			token=TK_ADD;
			ch=getCh();
			break;
		case '-':
			token=TK_SUB;
			ch=getCh();
			break;
		case '*':
			token=TK_MUL;
			ch=getCh();
			break;
		case '/':
			token=TK_DIV;
			ch=getCh();
			break;
		case '%':
			token=TK_MOD;
			ch=getCh();
			break;
		case '=':
			ch=getCh();
			if(ch=='=')
			{
				token=TK_EQ;
				ch=getCh();
			}
			else
			{
				token=TK_ASSIGN;
			}
			break;
		case '>':
			ch=getCh();
			if(ch=='=')
			{
				token=TK_GEQ;
				ch=getCh();
			}
			else
			{
				token=TK_GT;
			}
			break;
		case '<':
			ch=getCh();
			if(ch=='=')
			{
				token=TK_LEQ;
				ch=getCh();
			}
			else
			{
				token=TK_LT;
			}
			break;
		case '!':
			ch=getCh();
			if(ch=='=')
			{
				token=TK_NEQ;
				ch=getCh();
			}
			else
			{
				error((char*)"暂不支持'!'运算符",File,lineno,linepos);
			}
			break;
		case '&':
			ch=getCh();
			if(ch=='&')
			{
				token=TK_AND;
				ch=getCh();
			}
			else
			{
				error((char*)"暂不支持'&'",File,lineno,linepos);
			}
			break;
		case '|':
			ch=getCh();
			if(ch=='|')
			{
				token=TK_OR;
				ch=getCh();
			}
			else
			{
				error((char*)"暂不支持'|'",File,lineno,linepos);
			}
			break;
		case '(':
			token=TK_LLITTLE;
			ch=getCh();
			break;
		case ')':
			token=TK_RLITTLE;
			ch=getCh();
			break;
		case '[':
			token=TK_LMIDDLE;
			ch=getCh();
			break;
		case ']':
			token=TK_RMIDDLE;
			ch=getCh();
			break;
		case '{':
			token=TK_LLARGE;
			ch=getCh();
			break;
		case '}':
			token=TK_RLARGE;
			ch=getCh();
			break;
		case ';':
			token=TK_SEIM;
			ch=getCh();
			break;
		case ',':
			token=TK_COMMA;
			ch=getCh();
			break;
		case '.':
			token=TK_POINT;
			ch=getCh();
			break;
		case '"':
			getString();
			ch=getCh();
			token=TK_CONSTSTRING;
			break;
		case EOF:
			token=FL_EOF;
			break;
		default:
			error((char*)"不认识的字符'%c'",File,lineno,ch);
	}
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

char* getstr(TokenType t)
{
	if(t==TK_ID||token==TK_NUM)
	{
		return tokenstring;
	}
	for(int i=0;i<MAXTOKENSTR;i++)
	{
		if(tokenstr[i].tk==t)
		{
			return tokenstr[i].str;
		}
	}
}

void match(TokenType t)
{
	if(token!=t)
	{
		error((char*)"缺少'%s'",File,lineno,linepos,getstr(t));
	}
	gettoken();
}

TreeNode* newNode(NodeKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	for(int i=0;i<MAXCHILD;i++)
	{
		t->child[i]=NULL;
	}
	t->sibling=NULL;
	t->kind=kind;
	t->lineno=lineno;
	t->linepos=linepos;
	return t;
}

TreeNode* exp()
{
	TreeNode* t=exp_two();
	if(token==TK_OR)
	{
		TreeNode* p=newNode(Opk);
		p->child[0]=t;
		p->name=getstr(token);
		t=p;
		match(TK_OR);
		t->child[1]=exp_two();
	}
	return t;
}

TreeNode* exp_two()
{
	TreeNode* t=exp_three();
	if(token==TK_AND)
	{
		TreeNode* p=newNode(Opk);
		p->child[0]=t;
		p->name=getstr(token);
		t=p;
		match(TK_AND);
		t->child[1]=exp_three();
	}
	return t;
}

TreeNode* exp_three()
{
	TreeNode* t=exp_four();
	if(token==TK_EQ||token==TK_NEQ||token==TK_GT||token==TK_GEQ||token==TK_LT||token==TK_LEQ)
	{
		TreeNode* p=newNode(Opk);
		p->child[0]=t;
		p->name=getstr(token);
		t=p;
		match(token);
		t->child[1]=exp_four();
	}
	return t;
}

TreeNode* exp_four()
{
	TreeNode* t=exp_five();
	while(token==TK_ADD||token==TK_SUB)
	{
		TreeNode* p=newNode(Opk);
		p->child[0]=t;
		p->name=getstr(token);
		t=p;
		match(token);
		t->child[1]=exp_five();
	}
	return t;
}

TreeNode* exp_five()
{
	TreeNode* t=exp_six();
	while(token==TK_MUL||token==TK_DIV||token==TK_MOD)
	{
		TreeNode* p=newNode(Opk);
		p->name=getstr(token);
		p->child[0]=t;
		t=p;
		match(token);
		t->child[1]=exp_six();
	}
	return t;
}

TreeNode* exp_six()
{
	TreeNode* t=NULL;
	switch(token)
	{
		case KW_INT:
			t=newNode(IntK);
			match(KW_INT);
			if(token==TK_MUL)
			{
				t->kind=PointerK;
				t->child[0]=newNode(DataKindK);
				t->child[0]->name=(char*)"Int";
				match(TK_MUL);
			}
			t->name=copyString(tokenstring);
			match(TK_ID);
			if(token==TK_ASSIGN)
			{
				TreeNode* p=newNode(AssignK);
				p->name=t->name;
				match(TK_ASSIGN);
				p->child[0]=exp();
				t->sibling=p;
			}
			else if(token==TK_LMIDDLE)
			{
				t->kind=NewArrayK;
				match(TK_LMIDDLE);
				t->child[0]=newNode(DataKindK);
				t->child[0]->name=(char*)"Int";
				if(token!=TK_RMIDDLE)
				{
					t->child[1]=newNode(ArrayNumK);
					t->child[1]=exp();
				}
				match(TK_RMIDDLE);
			}
			else if(token==TK_LLITTLE)
			{
				match(TK_LLITTLE);
				t->child[1]=parameter();
				match(TK_RLITTLE);
				if(token==TK_LLARGE)
				{
					t->kind=FuncComeK;
					t->child[0]=newNode(DataKindK);
					t->child[0]->name=(char*)"Int";
					match(TK_LLARGE);
					if(token!=TK_RLARGE)
					{
						t->child[2]=block();
					}
					match(TK_RLARGE);
				}
				else
				{
					t->kind=FuncStateK;
					t->child[0]=newNode(DataKindK);
					t->child[0]->name=(char*)"Int";
				}
			}
			break;
		case KW_VOID:
			t=newNode(VoidK);
			match(KW_VOID);
			if(token==TK_MUL)
			{
				t->kind=PointerK;
				t->child[0]=newNode(DataKindK);
				t->child[0]->name=(char*)"Void";
				match(TK_MUL);
			}
			t->name=copyString(tokenstring);
			match(TK_ID);
			if(token==TK_LLITTLE)
			{
				match(TK_LLITTLE);
				t->child[1]=parameter();
				match(TK_RLITTLE);
				if(token==TK_LLARGE)
				{
					t->kind=FuncComeK;
					t->child[0]=newNode(DataKindK);
					t->child[0]->name=(char*)"Void";
					match(TK_LLARGE);
					if(token!=TK_RLARGE)
					{
						t->child[2]=block();
					}
					match(TK_RLARGE);
				}
				else
				{
					t->kind=FuncStateK;
					t->child[0]=newNode(DataKindK);
					t->child[0]->name=(char*)"Void";
				}
			}
			break;
		case TK_NUM:
			t=newNode(Numk);
			t->name=copyString(tokenstring);
			match(TK_NUM);
			break;
		case TK_ID:
			t=newNode(Idk);
			t->name=copyString(tokenstring);
			match(TK_ID);
			if(token==TK_LLITTLE)
			{
				t->kind=CallK;
				match(TK_LLITTLE);
				t->child[0]=parameter();
				match(TK_RLITTLE);
			}
			else if(token==TK_ASSIGN)
			{
				t->kind=AssignK;
				match(TK_ASSIGN);
				t->child[0]=exp();
			}
			else if(token==TK_LMIDDLE)
			{
				t->kind=ArrayK;
				TreeNode* p=newNode(ArrayNumK);
				match(TK_LMIDDLE);
				if(token!=TK_RMIDDLE)
				{
					p->child[0]=exp();
					t->child[0]=p;
				}
				match(TK_RMIDDLE);
			}
			else if(token==TK_POINT)
			{
				t->kind=MemberK;
				match(TK_POINT);
				t->child[0]=exp();
			}
			else if(token==TK_ID)
			{
				t->child[0]=newNode(DataKindK);
				t->child[0]->name=t->name;
				t->name=copyString(tokenstring);
				match(TK_ID);
				if(token==TK_LLITTLE)
				{
					match(TK_LLITTLE);
					t->child[1]=parameter();
					match(TK_RLITTLE);
					if(token==TK_LLARGE)
					{
						t->kind=FuncComeK;
						match(TK_LLARGE);
						if(token!=TK_RLARGE)
						{
							t->child[2]=block();
						}
						match(TK_RLARGE);
					}
					else
					{
						t->kind=FuncStateK;
					}
				}
				else if(token==TK_LMIDDLE)
				{
					t->kind=NewArrayK;
					match(TK_LMIDDLE);
					TreeNode* p=newNode(ArrayNumK);
					p->child[0]=exp();
					t->child[1]=p;
					match(TK_RMIDDLE);
				}
				else
				{
					t->kind=UserVarK;
				}
			}
			else if(token==TK_LLARGE)
			{
				match(TK_LLARGE);
				t->kind=FuncStateK;
				match(TK_RLARGE);
			}
			break;
		case TK_LLITTLE:
			match(TK_LLITTLE);
			if(token!=TK_RLITTLE)
			{
				t=exp();
			}
			match(TK_RLITTLE);
			break;
		case TK_CONSTSTRING:
			t=newNode(ConstStringK);
			t->name=copyString(tokenstring);
			match(TK_CONSTSTRING);
			break;
		case TK_MUL:
			t=newNode(PointerK);
			match(TK_MUL);
			t->name=copyString(tokenstring);
			match(TK_ID);
			break;
		default:
			error((char*)"不符合的类型'%s'",File,lineno,linepos,getstr(token));
	}
	return t;
}

TreeNode* parameter()
{
	TreeNode* t=NULL;
	if(token!=TK_RLITTLE)
	{
		t=newNode(ParamK);
		t->child[0]=exp();
		TreeNode* p=t;
		while(token==TK_COMMA)
		{
			TreeNode* q=newNode(ParamK);
			match(TK_COMMA);
			q->child[0]=exp();
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

TreeNode* int_stmt()
{
	TreeNode* t=newNode(IntK);
	bool point=false;
	match(KW_INT);
	if(token==TK_MUL)
	{
		t->kind=NewPointerK;
		t->child[0]=newNode(DataKindK);
		t->child[0]->name=(char*)"Int";
		match(TK_MUL);
		point=true;
	}
	t->name=copyString(tokenstring);
	match(TK_ID);
	if(token==TK_LLITTLE)
	{
		match(TK_LLITTLE);
		t->child[1]=parameter();
		match(TK_RLITTLE);
		if(token==TK_LLARGE)
		{
			t->kind=FuncComeK;
			t->child[0]=newNode(DataKindK);
			t->child[0]->name=(char*)"Int";
			match(TK_LLARGE);
			if(token!=TK_RLARGE)
			{
				t->child[2]=block();
			}
			match(TK_RLARGE);
		}
		else
		{
			t->kind=FuncStateK;
			t->child[0]=newNode(DataKindK);
			t->child[0]->name=(char*)"Int";
			match(TK_SEIM);
		}
	}
	else if(token==TK_ASSIGN&&point==false)
	{
		TreeNode* p=newNode(AssignK);
		p->name=t->name;
		match(TK_ASSIGN);
		p->child[0]=exp();
		t->sibling=p;
		match(TK_SEIM);
	}
	else if(token==TK_ASSIGN&&point==true)
	{
		TreeNode* p=newNode(PointerAssignK);
		p->name=t->name;
		match(TK_ASSIGN);
		p->child[0]=exp();
		t->sibling=p;
		match(TK_SEIM);
	}
	else if(token==TK_LMIDDLE)
	{
		t->kind=NewArrayK;
		t->child[0]=newNode(DataKindK);
		t->child[0]->name=(char*)"Int";
		match(TK_LMIDDLE);
		TreeNode* p=newNode(ArrayNumK);
		p->child[0]=exp();
		t->child[1]=p;
		match(TK_RMIDDLE);
		match(TK_SEIM);
	}
	else
	{
		match(TK_SEIM);
	}
	return t;
}

TreeNode* void_stmt()
{
	TreeNode* t=newNode(VoidK);
	match(KW_VOID);
	if(token==TK_MUL)
	{
		t->kind=NewPointerK;
		t->child[0]=newNode(DataKindK);
		t->child[0]->name=(char*)"Void";
		match(TK_MUL);
	}
	t->name=copyString(tokenstring);
	match(TK_ID);
	if(token==TK_LLITTLE)
	{
		match(TK_LLITTLE);
		t->child[1]=parameter();
		match(TK_RLITTLE);
		if(token==TK_LLARGE)
		{
			t->kind=FuncComeK;
			t->child[0]=newNode(DataKindK);
			t->child[0]->name=(char*)"Void";
			match(TK_LLARGE);
			if(token!=TK_RLARGE)
			{
				t->child[2]=block();
			}
			match(TK_RLARGE);
		}
		else
		{
			t->kind=FuncStateK;
			t->child[0]=newNode(DataKindK);
			t->child[0]->name=(char*)"Void";
			match(TK_SEIM);
		}
	}
	else
	{
		match(TK_SEIM);
	}
	return t;
}

TreeNode* string_stmt()
{
	TreeNode* t=newNode(StringK);
	match(KW_STRING);
	t->name=copyString(tokenstring);
	match(TK_ID);
	if(token==TK_LLITTLE)
	{
		match(TK_LLITTLE);
		t->child[1]=parameter();
		match(TK_RLITTLE);
		if(token==TK_LLARGE)
		{
			t->kind=FuncComeK;
			t->child[0]=newNode(DataKindK);
			t->child[0]->name=(char*)"String";
			match(TK_LLARGE);
			if(token!=TK_RLARGE)
			{
				t->child[2]=block();
			}
			match(TK_RLARGE);
		}
		else
		{
			t->kind=FuncStateK;
			t->child[0]=newNode(DataKindK);
			t->child[0]->name=(char*)"String";
			match(TK_SEIM);
		}
	}
	else if(token==TK_ASSIGN)
	{
		TreeNode* p=newNode(AssignK);
		p->name=t->name;
		match(TK_ASSIGN);
		p->child[0]=exp();
		t->sibling=p;
		match(TK_SEIM);
	}
	else if(token==TK_LMIDDLE)
	{
		t->kind=NewArrayK;
		t->child[0]=newNode(DataKindK);
		t->child[0]->name=(char*)"String";
		match(TK_LMIDDLE);
		TreeNode* p=newNode(ArrayNumK);
		p->child[0]=exp();
		t->child[1]=p;
		match(TK_RMIDDLE);
		match(TK_SEIM);
	}
	else
	{
		match(TK_SEIM);
	}
	return t;
}

TreeNode* if_stmt()
{
	TreeNode* t=newNode(Ifk);
	match(KW_IF);
	match(TK_LLITTLE);
	t->child[0]=exp();
	match(TK_RLITTLE);
	t->child[1]=sentence();
	if(token==KW_ELSE)
	{
		match(KW_ELSE);
		t->child[2]=sentence();
	}
	return t;
}

TreeNode* for_stmt()
{
	TreeNode* t=newNode(ForK);
	match(KW_FOR);
	match(TK_LLITTLE);
	if(token!=TK_SEIM)
	{
		t->child[0]=exp();
	}
	match(TK_SEIM);
	if(token!=TK_SEIM)
	{
		t->child[1]=exp();
	}
	match(TK_SEIM);
	if(token!=TK_RLITTLE)
	{
		t->child[2]=exp();
	}
	match(TK_RLITTLE);
	t->child[3]=sentence();
	return t;
}

TreeNode* while_stmt()
{
	TreeNode* t=newNode(WhileK);
	match(KW_WHILE);
	match(TK_LLITTLE);
	t->child[0]=exp();
	match(TK_RLITTLE);
	t->child[1]=sentence();
	return t;
}

TreeNode* do_stmt()
{
	TreeNode* t=newNode(DoWhileK);
	match(KW_DO);
	match(TK_LLARGE);
	t->child[0]=block();
	match(TK_RLARGE);
	match(KW_WHILE);
	match(TK_LLITTLE);
	t->child[1]=exp();
	match(TK_RLITTLE);
	match(TK_SEIM);
	return t;
}

TreeNode* break_stmt()
{
	TreeNode* t=newNode(BreaKK);
	match(KW_BREAK);
	match(TK_SEIM);
	return t;
}

TreeNode* continue_stmt()
{
	TreeNode* t=newNode(ContinueK);
	match(KW_CONTINUE);
	match(TK_SEIM);
	return t;
}

TreeNode* assign_stmt()
{
	TreeNode* t=newNode(AssignK);
	if(token==TK_MUL)
	{
		t->kind=PointerAssignK;
		match(TK_MUL);
	}
	t->name=copyString(tokenstring);
	match(TK_ID);
	if(token==TK_ASSIGN)
	{
		match(TK_ASSIGN);
		t->child[0]=exp();
		match(TK_SEIM);
	}
	else if(token==TK_LLITTLE)
	{
		t->kind=CallK;
		match(TK_LLITTLE);
		t->child[0]=parameter();
		match(TK_RLITTLE);
		match(TK_SEIM);
	}
	else if(token==TK_LMIDDLE)
	{
		t->kind=ArrayAssignK;
		match(TK_LMIDDLE);
		TreeNode* p=newNode(ArrayNumK);
		p->child[0]=exp();
		t->child[0]=p;
		match(TK_RMIDDLE);
		match(TK_ASSIGN);
		t->child[1]=exp();
		match(TK_SEIM);
	}
	else if(token==TK_POINT)
	{
		t->kind=MemberK;
		match(TK_POINT);
		t->child[0]=exp();
		match(TK_SEIM);
	}
	else if(token==TK_ID)
	{
		t->child[0]=newNode(DataKindK);
		t->child[0]->name=t->name;
		t->name=copyString(tokenstring);
		match(TK_ID);
		if(token==TK_LLITTLE)
		{
			match(TK_LLITTLE);
			t->child[1]=parameter();
			match(TK_RLITTLE);
			if(token==TK_LLARGE)
			{
				t->kind=FuncComeK;
				match(TK_LLARGE);
				if(token!=TK_RLARGE)
				{
					t->child[2]=block();
				}
				match(TK_RLARGE);
			}
			else
			{
				t->kind=FuncStateK;
				match(TK_SEIM);
			}
		}
		else if(token==TK_LMIDDLE)
		{
			t->kind=NewArrayK;
			match(TK_LMIDDLE);
			TreeNode* p=newNode(ArrayNumK);
			p->child[0]=exp();
			t->child[1]=p;
			match(TK_RMIDDLE);
			match(TK_SEIM);
		}
		else
		{
			t->kind=UserVarK;
			match(TK_SEIM);
		}
	}
	else if(token==TK_MUL)
	{
		match(TK_MUL);
		t->kind=NewPointerK;
		t->child[0]=newNode(DataKindK);
		t->child[0]->name=t->name;
		t->name=copyString(tokenstring);
		match(TK_ID);
		if(token==TK_ASSIGN)
		{
			TreeNode* p=newNode(PointerAssignK);
			p->name=t->name;
			match(TK_ASSIGN);
			p->child[0]=exp();
			t->sibling=p;
		}
		match(TK_SEIM);
	}
	else
	{
		match(TK_SEIM);
	} 
	return t;
}

TreeNode* return_stmt()
{
	TreeNode* t=newNode(ReturnK);
	match(KW_RETURN);
	t->child[0]=exp();
	match(TK_SEIM);
	return t;
}

TreeNode* print_stmt()
{
	TreeNode* t=newNode(PrintK);
	match(KW_PRINT);
	match(TK_LLITTLE);
	t->child[0]=exp();
	TreeNode* p=t;
	while(token==TK_COMMA)
	{
		match(TK_COMMA);
		TreeNode* q=newNode(PrintK);
		q->child[0]=exp();
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
	match(TK_RLITTLE);
	match(TK_SEIM);
	return t;
}

TreeNode* input_stmt()
{
	TreeNode* t=newNode(InputK);
	match(KW_INPUT);
	match(TK_LLITTLE);
	t->child[0]=exp();
	TreeNode* p=t;
	while(token==TK_COMMA)
	{
		match(TK_COMMA);
		TreeNode* q=newNode(InputK);
		q->child[0]=exp();
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
	match(TK_RLITTLE);
	match(TK_SEIM);
	return t;
}

TreeNode* struct_stmt()
{
	TreeNode* t=newNode(StructK);
	match(KW_STRUCT);
	t->name=copyString(tokenstring);
	match(TK_ID);
	match(TK_LLARGE);
	t->child[0]=block();
	match(TK_RLARGE);
	match(TK_SEIM);
	return t;
}

TreeNode* sentence()
{
	TreeNode* t=NULL;
	switch(token)
	{
		case KW_INT:
			t=int_stmt();
			break;
		case KW_VOID:
			t=void_stmt();
			break;
		case KW_RETURN:
			t=return_stmt();
			break;
		case TK_LLARGE:
			match(TK_LLARGE);
			if(token!=TK_RLARGE)
			{
				t=block();
			}
			match(TK_RLARGE);
			break;
		case KW_IF:
			t=if_stmt();
			break;
		case KW_FOR:
			t=for_stmt();
			break;
		case KW_WHILE:
			t=while_stmt();
			break;
		case KW_DO:
			t=do_stmt();
			break;
		case KW_BREAK:
			t=break_stmt();
			break;
		case KW_CONTINUE:
			t=continue_stmt();
			break;
		case TK_MUL:
		case TK_ID:
			t=assign_stmt();
			break;
		case TK_SEIM:
			match(TK_SEIM);
			break;
		case KW_PRINT:
			t=print_stmt();
			break;
		case KW_INPUT:
			t=input_stmt();
			break;
		case KW_STRUCT:
			t=struct_stmt();
			break;
		case KW_STRING:
			t=string_stmt();
			break;
		default:
			error((char*)"'%s'不能单独出现",File,lineno,linepos,getstr(token));
	}
	return t;
}

TreeNode* block()
{
	TreeNode* t=sentence();
	TreeNode* p=t;
	while(p!=NULL&&p->sibling!=NULL)
	{
		p=p->sibling;
	}
	while(token!=FL_EOF&&token!=TK_RLARGE)
	{
		TreeNode* q=NULL;
		q=sentence();
		if(t==NULL)
		{
			t=p=q;
		}
		else
		{
			p->sibling=q;
			while(q->sibling!=NULL)
			{
				q=q->sibling;
			}
			p=q;
		}
	}
	return t;
}

TreeNode* compiler(char* f,vector<char> filei)
{
	lineno=1;
	File=f;
	maxlen=filei.size();
	file=new char[maxlen+1];
	for(int i=0;i<maxlen;i++)
	{
		file[i]=filei[i];
	}
	ch=getCh();
	gettoken();
	if(token==FL_EOF)
	{
		printf("%s: 文件为空\n",File);
		exit(-1);
	}
	TreeNode* t=block();
	return t;
}
