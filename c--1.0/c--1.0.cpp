#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

#pragma warning(disable:4996)

#define KEYWORDSLEN 20
#define REGLEN 11

#define isval(ch) ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_')
#define isnum(ch) (ch>='0'&&ch<='9')
#define iswhite(ch) (ch==' '||ch=='\t'||ch=='\n')

enum Token
{
	NAME, NUM, STRING,
	ASSIGN, OR, AND, EQ, NEQ, GT, GEQ, LT, LEQ,
	LLITTLE, RLITTLE, LMIDDLE, RMIDDLE, LLARGE, RLARGE,
	SUB, ADD, MUL, DIV, MOD,
	IF, ELSE, WHILE, FOR, FUNCTION, RETURN, BREAK, CONTINUE, PRINT, INPUT,
	SEMI, COMMA, WELL,
	SINGLELINECOMMENTS, MULTILINECOMMENT,
	DEFINE, INCLUDE, UNDEF, IFDEF, ENDIF, IFNDEF, ELIFDEF, ELSEDEF,
	END
};

string code, name, unreplacecode;
int linepos = 0;
Token token;
char ch;
string include_path = "";
bool ishell = true;
FILE* infile;

enum NodeKind
{
	NameK, AssignK, NumK, OrK, AndK, SubK, AddK, MulK, DivK, ModK, EqK, NeqK, GtK, GeqK, LtK, LeqK, IfK, ElseK, WhileK, ForK, FuncDefK, SingleLineCommentsK, MultilinecommentK, CallK, ArgK,
	CallArgK, StringK, ListIndexK, ReturnK, ContinueK, BreakK, DefineK, IncludeK, UndefK, IfdefK, IfndefK, ElifdefK, ElsedefK, ListK, PrintK, InputK, EndifK
};

struct SyntaxTree
{
	NodeKind nodekind;
	SyntaxTree* next;
	vector<SyntaxTree*> child;
	string val;
	string code;
};

struct Define
{
	string name;
	string content;
};
vector<Define> define;

struct Data
{
	string name;	//变量名
	string type;	//变量类型
	string content;	//变量值(如果有)
	SyntaxTree* function;	//函数
	vector<Data*> members;	//变量属性或数组
};
vector<vector<Data*> > datas;	//数据区
Data* reg[REGLEN];	//寄存器
vector<Data*> args;	//函数参数栈
int nameindex;	//变量索引
int definebool;	//宏判断

struct KeyWords
{
	string keyword;
	Token tk;
}KEYWORDS[KEYWORDSLEN] = { { "or",OR },{"if",IF},{"while",WHILE},{"else",ELSE},{"for",FOR},{"function",FUNCTION},{"and",AND},{"return",RETURN},{"continue",CONTINUE},
							{"break",BREAK},{"define",DEFINE},{"include",INCLUDE} ,{"undef",UNDEF} ,{"ifdef",IFDEF},{"endif",ENDIF},{"ifndef",IFNDEF},{"elif",ELIFDEF},
							{"elsedef",ELSEDEF},{"print",PRINT},{"input",INPUT} };

SyntaxTree* exp();
SyntaxTree* block();
SyntaxTree* sentence();
SyntaxTree* getcallarg();
SyntaxTree* tosyntaxtree();
SyntaxTree* input_sentence();
void printtree(SyntaxTree*, int);
void shell();

/*
函数: newsyntaxtree
	功能:
		创建一个语法树节点
	参数:
		nodekind:
			节点类型
		val:
			节点值
	返回值:
		语法树节点
*/
SyntaxTree* newsyntaxtree(NodeKind nodekind, string val)
{
	SyntaxTree* syntaxtree = new SyntaxTree;
	syntaxtree->nodekind = nodekind;
	syntaxtree->next = NULL;
	syntaxtree->val = val;
	syntaxtree->code = code;
	return syntaxtree;
}

void error(string message, string code)
{
	cout << code << ": 错误: " << message << endl;
	if (ishell)
	{
		shell();
	}
	else
	{
		exit(1);
	}
}

/*
函数: replace_all_distinct
	功能: 替换字符串
	参数:
		str:
			源字符串
		old_value:
			替换内容
		new_value:
			新内容
	返回值:
		替换后的字符串
*/
string& replace_all_distinct(string& str, const  string& old_value, const   string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else
		{
			break;
		}
	}
	return   str;
}

/*
函数: define_replace
	功能: 宏替换
*/
void define_replace()
{
	for (int i = 0; i < define.size(); i++)
	{
		code = replace_all_distinct(code, define[i].name, define[i].content);
	}
}

void readcode(string out)
{
	cout << out;
	getline(cin, code);
	unreplacecode = code;
	define_replace();
}

/*
函数: getch
	功能: 获取一个字符
	返回值:
		字符
*/
char getch()
{
	if (linepos < code.size())
	{
		return code[linepos++];
	}
	else if (ishell)
	{
		readcode("... ");
		linepos = 0;
		return code[linepos++];
	}
	else
	{
		char buf[1024];
		if (fgets(buf, 1024, infile) != NULL)
		{
			code = buf;
			unreplacecode = code;
			define_replace();
			linepos = 0;
			return code[linepos++];
		}
		else
		{
			return 0;
		}
	}
}

/*
函数: ungetch
	功能: 返回上一次读取的字符
*/
void ungetch()
{
	linepos--;
}

/*
函数: getname
	功能: 获取变量名
	返回值:
		Token
*/
Token getname()
{
	name = "";
	while ((isval(ch) || isnum(ch)) && ch != EOF)
	{
		name += ch;
		ch = getch();
	}
	for (int i = 0; i < KEYWORDSLEN; i++)
	{
		if (name == KEYWORDS[i].keyword)
		{
			return KEYWORDS[i].tk;
		}
	}
	return NAME;
}

/*
函数: getnum
	功能: 获取数字
	返回值:
		Token
*/
Token getnum()
{
	name = "";
	while (isnum(ch) && ch != EOF)
	{
		name += ch;
		ch = getch();
	}
	return NUM;
}

/*
函数: gettoken
	功能: 获取token
*/
void gettoken()
{
	while (iswhite(ch))
	{
		ch = getch();
	}
	if (isval(ch))
	{
		token = getname();
	}
	else if (isnum(ch))
	{
		token = getnum();
	}
	else if (ch == '=')
	{
		ch = getch();
		if (ch == '=')
		{
			token = EQ;
			ch = getch();
		}
		else
		{
			token = ASSIGN;
		}
	}
	else if (ch == '!')
	{
		ch = getch();
		if (ch == '=')
		{
			token = NEQ;
			ch = getch();
		}
	}
	else if (ch == '>')
	{
		ch = getch();
		if (ch == '=')
		{
			token = GEQ;
			ch = getch();
		}
		else
		{
			token = GT;
		}
	}
	else if (ch == '<')
	{
		ch = getch();
		if (ch == '=')
		{
			token = LEQ;
			ch = getch();
		}
		else
		{
			token = LT;
		}
	}
	else if (ch == '+')
	{
		token = ADD;
		ch = getch();
	}
	else if (ch == '-')
	{
		token = SUB;
		ch = getch();
	}
	else if (ch == '*')
	{
		token = MUL;
		ch = getch();
	}
	else if (ch == '/')
	{
		ch = getch();
		if (ch == '/')
		{
			token = SINGLELINECOMMENTS;
			ch = getch();
			name = "";
			while (ch != 0 && ch != EOF)
			{
				name += ch;
				ch = getch();
			}
		}
		else if (ch == '*')
		{
			token = MULTILINECOMMENT;
			ch = getch();
			name = "";
			while (true)
			{
				if (ch == '*')
				{
					ch = getch();
					if (ch == '/')
					{
						ch = getch();
						break;
					}
					else
					{
						ungetch();
					}
				}
				name += ch;
				ch = getch();
			}
		}
		else
		{
			token = DIV;
		}
	}
	else if (ch == '%')
	{
		token = MOD;
		ch = getch();
	}
	else if (ch == '(')
	{
		token = LLITTLE;
		ch = getch();
	}
	else if (ch == ')')
	{
		token = RLITTLE;
		ch = getch();
	}
	else if (ch == '[')
	{
		token = LMIDDLE;
		ch = getch();
	}
	else if (ch == ']')
	{
		token = RMIDDLE;
		ch = getch();
	}
	else if (ch == '{')
	{
		token = LLARGE;
		ch = getch();
	}
	else if (ch == '}')
	{
		token = RLARGE;
		ch = getch();
	}
	else if (ch == ';')
	{
		token = SEMI;
		ch = getch();
	}
	else if (ch == ',')
	{
		token = COMMA;
		ch = getch();
	}
	else if (ch == '#')
	{
		token = WELL;
		ch = getch();
	}
	else if (ch == '"')
	{
		token = STRING;
		ch = getch();
		name = "";
		while (ch != '"' && ch != EOF)
		{
			if (ch == '\\')
			{
				ch = getch();
				switch (ch)
				{
				case 'n':
					ch = '\n';
					break;
				case 't':
					ch = '\t';
					break;
				case '\\':
					ch = '\\';
					break;
				case '\'':
					ch = '\'';
					break;
				case '"':
					ch = '"';
					break;
				default:
					break;
				}
			}
			name += ch;
			ch = getch();
		}
		ch = getch();
	}
	else if (ch == '\'')
	{
		token = STRING;
		ch = getch();
		name = "";
		while (ch != '\'' && ch != EOF)
		{
			if (ch == '\\')
			{
				ch = getch();
				switch (ch)
				{
				case 'n':
					ch = '\n';
					break;
				case 't':
					ch = '\t';
					break;
				case '\\':
					ch = '\\';
					break;
				case '\'':
					ch = '\'';
					break;
				case '"':
					ch = '"';
					break;
				default:
					break;
				}
			}
			name += ch;
			ch = getch();
		}
		ch = getch();
	}
	else
	{
		token = END;
	}
}

/*
函数: match
	功能: 匹配token
	参数:
		matchtoken:
			匹配的token
*/
void match(Token matchtoken)
{
	if (token == matchtoken)
	{
		gettoken();
	}
	else
	{
		error("缺少符号", code);
	}
}

SyntaxTree* exp_six()
{
	SyntaxTree* t = NULL;
	if (token == NUM)
	{
		t = newsyntaxtree(NumK, name);
		match(token);
	}
	else if (token == NAME)
	{
		t = newsyntaxtree(NameK, name);
		match(token);
		if (token == LLITTLE)
		{
			match(LLITTLE);
			SyntaxTree* p = newsyntaxtree(CallK, "");
			p->child.push_back(t);
			p->child.push_back(getcallarg());
			match(RLITTLE);
			t = p;
		}
		else if (token == LMIDDLE)
		{
			while (token == LMIDDLE)
			{
				match(LMIDDLE);
				SyntaxTree* p = newsyntaxtree(ListIndexK, "");
				p->child.push_back(t);
				p->child.push_back(exp());
				match(RMIDDLE);
				t = p;
			}
		}
	}
	else if (token == STRING)
	{
		t = newsyntaxtree(StringK, name);
		match(token);
	}
	else if (token == LLITTLE)
	{
		match(LLITTLE);
		if (token != RLITTLE)
		{
			t = exp();
		}
		match(RLITTLE);
	}
	else if (token == LMIDDLE)
	{
		t = newsyntaxtree(ListK, "");
		match(token);
		if (token != RMIDDLE)
		{
			t->child.push_back(exp());
			while (token == COMMA)
			{
				match(COMMA);
				t->child.push_back(exp());
			}
		}
		match(RMIDDLE);
	}
	else if (token == INPUT)
	{
		t = input_sentence();
	}
	return t;
}

SyntaxTree* exp_five()
{
	SyntaxTree* t = exp_six();
	while (token == MUL || token == DIV || token == MOD)
	{
		if (token == MUL)
		{
			SyntaxTree* p = newsyntaxtree(MulK, "");
			p->child.push_back(t);
			match(token);
			p->child.push_back(exp_six());
			t = p;
		}
		else if (token == DIV)
		{
			SyntaxTree* p = newsyntaxtree(DivK, "");
			p->child.push_back(t);
			match(token);
			p->child.push_back(exp_six());
			t = p;
		}
		else if (token == MOD)
		{
			SyntaxTree* p = newsyntaxtree(ModK, "");
			p->child.push_back(t);
			match(token);
			p->child.push_back(exp_six());
			t = p;
		}
	}
	return t;
}

SyntaxTree* exp_four()
{
	SyntaxTree* t = exp_five();
	while (token == ADD || token == SUB)
	{
		if (token == ADD)
		{
			SyntaxTree* p = newsyntaxtree(AddK, "");
			p->child.push_back(t);
			match(token);
			p->child.push_back(exp_five());
			t = p;
		}
		else if (token == SUB)
		{
			SyntaxTree* p = newsyntaxtree(SubK, "");
			p->child.push_back(t);
			match(token);
			p->child.push_back(exp_five());
			t = p;
		}
	}
	return t;
}

SyntaxTree* exp_three()
{
	SyntaxTree* t = exp_four();
	if (token == EQ)
	{
		SyntaxTree* p = newsyntaxtree(EqK, "");
		p->child.push_back(t);
		match(token);
		p->child.push_back(exp_four());
		t = p;
	}
	else if (token == NEQ)
	{
		SyntaxTree* p = newsyntaxtree(NeqK, "");
		p->child.push_back(t);
		match(token);
		p->child.push_back(exp_four());
		t = p;
	}
	else if (token == GT)
	{
		SyntaxTree* p = newsyntaxtree(GtK, "");
		p->child.push_back(t);
		match(token);
		p->child.push_back(exp_four());
		t = p;
	}
	else if (token == GEQ)
	{
		SyntaxTree* p = newsyntaxtree(GeqK, "");
		p->child.push_back(t);
		match(token);
		p->child.push_back(exp_four());
		t = p;
	}
	else if (token == LT)
	{
		SyntaxTree* p = newsyntaxtree(LtK, "");
		p->child.push_back(t);
		match(token);
		p->child.push_back(exp_four());
		t = p;
	}
	else if (token == LEQ)
	{
		SyntaxTree* p = newsyntaxtree(LeqK, "");
		p->child.push_back(t);
		match(token);
		p->child.push_back(exp_four());
		t = p;
	}
	return t;
}

SyntaxTree* exp_two()
{
	SyntaxTree* t = exp_three();
	if (token == AND)
	{
		SyntaxTree* p = newsyntaxtree(AndK, "");
		p->child.push_back(t);
		match(token);
		p->child.push_back(exp_three());
		t = p;
	}
	return t;
}

SyntaxTree* exp()
{
	SyntaxTree* t = exp_two();
	if (token == OR)
	{
		SyntaxTree* p = newsyntaxtree(OrK, "");
		p->child.push_back(t);
		match(token);
		p->child.push_back(exp_two());
		t = p;
	}
	return t;
}

SyntaxTree* getcallarg()
{
	SyntaxTree* t = NULL;
	if (token != RLITTLE)
	{
		t = newsyntaxtree(CallArgK, "");
		t->child.push_back(exp());
		while (token == COMMA)
		{
			match(COMMA);
			t->child.push_back(exp());
		}
	}
	return t;
}

SyntaxTree* assign_sentence()
{
	SyntaxTree* t = newsyntaxtree(NameK, name);
	match(NAME);
	if (token == ASSIGN)
	{
		match(ASSIGN);
		SyntaxTree* p = newsyntaxtree(AssignK, "");
		p->child.push_back(t);
		p->child.push_back(exp());
		t = p;
	}
	else if (token == LLITTLE)
	{
		match(LLITTLE);
		SyntaxTree* p = newsyntaxtree(CallK, "");
		p->child.push_back(t);
		p->child.push_back(getcallarg());
		match(RLITTLE);
		t = p;
	}
	else if (token == LMIDDLE)
	{
		while (token == LMIDDLE)
		{
			match(LMIDDLE);
			SyntaxTree* p = newsyntaxtree(ListIndexK, "");
			p->child.push_back(t);
			p->child.push_back(exp());
			match(RMIDDLE);
			t = p;
		}
		if (token == ASSIGN)
		{
			SyntaxTree* p = newsyntaxtree(AssignK, "");
			p->child.push_back(t);
			match(ASSIGN);
			p->child.push_back(exp());
			t = p;
		}
	}
	return t;
}

SyntaxTree* if_sentence()
{
	SyntaxTree* t = newsyntaxtree(IfK, "");
	match(IF);
	t->child.push_back(exp());
	t->child.push_back(sentence());
	if (token == ELSE)
	{
		match(ELSE);
		SyntaxTree* p = newsyntaxtree(ElseK, "");
		p->child.push_back(sentence());
		t->child.push_back(p);
	}
	return t;
}

SyntaxTree* while_sentence()
{
	SyntaxTree* t = newsyntaxtree(WhileK, "");
	match(WHILE);
	t->child.push_back(exp());
	t->child.push_back(sentence());
	return t;
}

SyntaxTree* getarg()
{
	SyntaxTree* t = NULL;
	if (token != RLITTLE)
	{
		t = newsyntaxtree(ArgK, "");
		t->child.push_back(exp());
		while (token == COMMA)
		{
			match(COMMA);
			t->child.push_back(exp());
		}
	}
	return t;
}

SyntaxTree* function_sentence()
{
	SyntaxTree* t = newsyntaxtree(FuncDefK, "");
	match(FUNCTION);
	t->child.push_back(newsyntaxtree(NameK, name));
	match(token);
	match(LLITTLE);
	if (token != RLITTLE)
	{
		t->child.push_back(getarg());
	}
	match(RLITTLE);
	t->child.push_back(sentence());
	return t;
}

SyntaxTree* return_sentence()
{
	SyntaxTree* t = newsyntaxtree(ReturnK, "");
	match(RETURN);
	t->child.push_back(exp());
	return t;
}

SyntaxTree* break_sentence()
{
	SyntaxTree* t = newsyntaxtree(BreakK, "");
	match(BREAK);
	return t;
}

SyntaxTree* continue_sentence()
{
	SyntaxTree* t = newsyntaxtree(ContinueK, "");
	match(CONTINUE);
	return t;
}

SyntaxTree* define_sentence()
{
	match(DEFINE);
	while (iswhite(ch))
	{
		ch = getch();
	}
	string content = "";
	while (ch != 0 && ch != '\n')
	{
		content += ch;
		ch = getch();
	}
	Define _define;
	_define.name = name;
	_define.content = content;
	define.push_back(_define);
	SyntaxTree* t = newsyntaxtree(DefineK, "");
	t->child.push_back(newsyntaxtree(NameK, name));
	t->child.push_back(newsyntaxtree(StringK, content));
	gettoken();
	return t;
}

/*
函数: readFileIntoString
	功能: 读取文件内容成字符串
	参数:
		filename:
			文件名
	返回值:
		文件内容
*/
string readFileIntoString(const char* filename)
{
	ifstream ifile(filename);
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
	{
		buf.put(ch);
	}
	return buf.str();
}

SyntaxTree* include_sentence()
{
	match(INCLUDE);
	SyntaxTree* t = newsyntaxtree(IncludeK, "");
	if (token == STRING)
	{
		t->child.push_back(newsyntaxtree(NameK, name));
		match(STRING);
	}
	else if (token == LT)
	{
		while (iswhite(ch))
		{
			ch = getch();
		}
		string filename = "";
		while (ch != '>')
		{
			filename += ch;
			ch = getch();
		}
		t->child.push_back(newsyntaxtree(NameK, include_path + filename));
	}
	return t;
}

SyntaxTree* undef_sentence()
{
	SyntaxTree* t = newsyntaxtree(UndefK, "");
	int _linepos = linepos;
	while (iswhite(ch))
	{
		ch = unreplacecode[_linepos++];
	}
	string _name = "";
	while (isnum(ch) || isval(ch))
	{
		_name += ch;
		ch = unreplacecode[_linepos++];
	}
	ch = code[linepos];
	match(UNDEF);
	t->child.push_back(newsyntaxtree(NameK, _name));
	for (int i = 0; i < define.size(); i++)
	{
		if (define[i].name == _name)
		{
			define.erase(define.begin() + i);
			break;
		}
	}
	gettoken();
	return t;
}

SyntaxTree* ifdef_sentence()
{
	SyntaxTree* t = newsyntaxtree(IfdefK, "");
	int _linepos = linepos;
	while (iswhite(ch))
	{
		ch = unreplacecode[_linepos++];
	}
	string _name = "";
	while (isnum(ch) || isval(ch))
	{
		_name += ch;
		ch = unreplacecode[_linepos++];
	}
	ch = code[linepos];
	gettoken();
	t->child.push_back(newsyntaxtree(NameK, _name));
	gettoken();
	SyntaxTree* p = NULL;
	SyntaxTree* pp = p;
	while (true)
	{
		if (token == END)
		{
			ch = getch();
			gettoken();
		}
		if (token == WELL)
		{
			break;
		}
		SyntaxTree* q = sentence();
		if (p == NULL)
		{
			p = pp = q;
		}
		else
		{
			pp->next = q;
			while (q != NULL && q->next != NULL)
			{
				q = q->next;
			}
			pp = q;
		}
	}
	t->child.push_back(p);
	//gettoken();
	return t;
}

SyntaxTree* ifndef_sentence()
{
	SyntaxTree* t = newsyntaxtree(IfndefK, "");
	int _linepos = linepos;
	while (iswhite(ch))
	{
		ch = unreplacecode[_linepos++];
	}
	string _name = "";
	while (isnum(ch) || isval(ch))
	{
		_name += ch;
		ch = unreplacecode[_linepos++];
	}
	ch = code[linepos];
	gettoken();
	t->child.push_back(newsyntaxtree(NameK, _name));
	gettoken();
	SyntaxTree* p = NULL;
	SyntaxTree* pp = p;
	while (true)
	{
		if (token == END)
		{
			ch = getch();
			gettoken();
		}
		if (token == WELL)
		{
			break;
		}
		SyntaxTree* q = sentence();
		if (p == NULL)
		{
			p = pp = q;
		}
		else
		{
			pp->next = q;
			while (q != NULL && q->next != NULL)
			{
				q = q->next;
			}
			pp = q;
		}
	}
	t->child.push_back(p);
	//gettoken();
	return t;
}

SyntaxTree* elifdef_sentence()
{
	SyntaxTree* t = newsyntaxtree(ElifdefK, "");
	int _linepos = linepos;
	while (iswhite(ch))
	{
		ch = unreplacecode[_linepos++];
	}
	string _name = "";
	while (isnum(ch) || isval(ch))
	{
		_name += ch;
		ch = unreplacecode[_linepos++];
	}
	ch = code[linepos];
	gettoken();
	t->child.push_back(newsyntaxtree(NameK, _name));
	gettoken();
	SyntaxTree* p = NULL;
	SyntaxTree* pp = p;
	while (true)
	{
		if (token == END)
		{
			ch = getch();
			gettoken();
		}
		if (token == WELL)
		{
			break;
		}
		SyntaxTree* q = sentence();
		if (p == NULL)
		{
			p = pp = q;
		}
		else
		{
			pp->next = q;
			while (q != NULL && q->next != NULL)
			{
				q = q->next;
			}
			pp = q;
		}
	}
	t->child.push_back(p);
	//gettoken();
	return t;
}

SyntaxTree* elsedef_sentence()
{
	SyntaxTree* t = newsyntaxtree(ElsedefK, "");
	match(ELSEDEF);
	SyntaxTree* p = NULL;
	SyntaxTree* pp = p;
	while (true)
	{
		if (token == END)
		{
			ch = getch();
			gettoken();
		}
		if (token == WELL)
		{
			break;
		}
		SyntaxTree* q = sentence();
		if (p == NULL)
		{
			p = pp = q;
		}
		else
		{
			pp->next = q;
			while (q != NULL && q->next != NULL)
			{
				q = q->next;
			}
			pp = q;
		}
	}
	t->child.push_back(p);
	//gettoken();
	return t;
}

SyntaxTree* pre_sentence()
{
	match(WELL);
	SyntaxTree* t = NULL;
	if (token == DEFINE)
	{
		t = define_sentence();
	}
	else if (token == INCLUDE)
	{
		t = include_sentence();
	}
	else if (token == UNDEF)
	{
		t = undef_sentence();
	}
	else if (token == IFDEF)
	{
		t = ifdef_sentence();
	}
	else if (token == IFNDEF)
	{
		t = ifndef_sentence();
	}
	else if (token == ELIFDEF)
	{
		t = elifdef_sentence();
	}
	else if (token == ELSEDEF)
	{
		t = elsedef_sentence();
	}
	else if (token == ENDIF)
	{
		t = newsyntaxtree(EndifK, "");
		match(ENDIF);
	}
	return t;
}

SyntaxTree* print_sentence()
{
	SyntaxTree* t = newsyntaxtree(PrintK, "");
	match(PRINT);
	match(LLITTLE);
	if (token != RLITTLE)
	{
		t->child.push_back(exp());
		while (token == COMMA)
		{
			match(token);
			t->child.push_back(exp());
		}
	}
	match(RLITTLE);
	return t;
}

SyntaxTree* input_sentence()
{
	SyntaxTree* t = newsyntaxtree(InputK, "");
	match(INPUT);
	match(LLITTLE);
	if (token != RLITTLE)
	{
		t->child.push_back(exp());
		while (token == COMMA)
		{
			match(token);
			t->child.push_back(exp());
		}
	}
	match(RLITTLE);
	return t;
}

SyntaxTree* sentence()
{
	SyntaxTree* t = NULL;
	if (token == NAME)
	{
		t = assign_sentence();
	}
	else if (token == IF)
	{
		t = if_sentence();
	}
	else if (token == WHILE)
	{
		t = while_sentence();
	}
	else if (token == FUNCTION)
	{
		t = function_sentence();
	}
	else if (token == LLARGE)
	{
		match(token);
		t = block();
		match(RLARGE);
	}
	else if (token == WELL)
	{
		t = pre_sentence();
	}
	else if (token == SINGLELINECOMMENTS)
	{
		t = newsyntaxtree(SingleLineCommentsK, name);
		match(token);
	}
	else if (token == MULTILINECOMMENT)
	{
		t = newsyntaxtree(MultilinecommentK, name);
		match(token);
	}
	else if (token == RETURN)
	{
		t = return_sentence();
	}
	else if (token == BREAK)
	{
		t = break_sentence();
	}
	else if (token == CONTINUE)
	{
		t = continue_sentence();
	}
	else if (token == PRINT)
	{
		t = print_sentence();
	}
	else if (token == INPUT)
	{
		t = input_sentence();
	}
	else if (token == SEMI)
	{
		match(token);
		t = sentence();
	}
	else
	{
		t = exp();
	}
	return t;
}

SyntaxTree* block()
{
	SyntaxTree* t = sentence();
	SyntaxTree* p = t;
	while (p != NULL && p->next != NULL)
	{
		p = p->next;
	}
	while (token != END && token != RLARGE)
	{
		SyntaxTree* q = sentence();
		if (t == NULL)
		{
			t = p = q;
		}
		else
		{
			p->next = q;
			while (q != NULL && q->next != NULL)
			{
				q = q->next;
			}
			p = q;
		}
	}
	return t;
}

SyntaxTree* tosyntaxtree()
{
	ch = getch();
	gettoken();
	return block();
}

void printtree(SyntaxTree* tree, int space)
{
	SyntaxTree* nowtree = tree;
	while (nowtree != NULL)
	{
		for (int i = 0; i < space; i++)
		{
			cout << "  ";
		}
		switch (nowtree->nodekind)
		{
		case NameK:
			cout << "Name: " << nowtree->val << endl;
			break;
		case NumK:
			cout << "Num: " << nowtree->val << endl;
			break;
		case AssignK:
			cout << "Assign" << endl;
			break;
		case OrK:
			cout << "Or" << endl;
			break;
		case AndK:
			cout << "And" << endl;
			break;
		case EqK:
			cout << "Eq" << endl;
			break;
		case NeqK:
			cout << "Neq" << endl;
			break;
		case GtK:
			cout << "Gt" << endl;
			break;
		case GeqK:
			cout << "Geq" << endl;
			break;
		case LtK:
			cout << "Lt" << endl;
			break;
		case LeqK:
			cout << "Leq" << endl;
			break;
		case AddK:
			cout << "Add" << endl;
			break;
		case SubK:
			cout << "Sub" << endl;
			break;
		case MulK:
			cout << "Mul" << endl;
			break;
		case DivK:
			cout << "Div" << endl;
			break;
		case ModK:
			cout << "Mod" << endl;
			break;
		case IfK:
			cout << "If" << endl;
			break;
		case ElseK:
			cout << "Else" << endl;
			break;
		case WhileK:
			cout << "While" << endl;
			break;
		case ForK:
			cout << "For" << endl;
			break;
		case FuncDefK:
			cout << "FuncDef" << endl;
			break;
		case SingleLineCommentsK:
			cout << "SingleLineComments: " << nowtree->val << endl;
			break;
		case MultilinecommentK:
			cout << "Multilinecomment: " << nowtree->val << endl;
			break;
		case CallK:
			cout << "Call" << endl;
			break;
		case ArgK:
			cout << "Arg" << endl;
			break;
		case CallArgK:
			cout << "CallArg" << endl;
			break;
		case StringK:
			cout << "String: " << nowtree->val << endl;
			break;
		case ListIndexK:
			cout << "ListIndex" << endl;
			break;
		case ReturnK:
			cout << "Return" << endl;
			break;
		case BreakK:
			cout << "Break" << endl;
			break;
		case ContinueK:
			cout << "Continue" << endl;
			break;
		case DefineK:
			cout << "Define" << endl;
			break;
		case IncludeK:
			cout << "Include" << endl;
			break;
		case UndefK:
			cout << "Undef" << endl;
			break;
		case IfdefK:
			cout << "Ifdef" << endl;
			break;
		case IfndefK:
			cout << "Ifndef" << endl;
			break;
		case ElifdefK:
			cout << "Elifdef" << endl;
			break;
		case ElsedefK:
			cout << "Elsedef" << endl;
			break;
		case ListK:
			cout << "List" << endl;
			break;
		case PrintK:
			cout << "Print" << endl;
			break;
		case InputK:
			cout << "Input" << endl;
			break;
		case EndifK:
			cout << "Endif" << endl;
			break;
		default:
			break;
		}
		for (int i = 0; i < nowtree->child.size(); i++)
		{
			printtree(nowtree->child[i], space + 1);
		}
		nowtree = nowtree->next;
	}
}

void printlist(vector<Data*> _data)
{
	cout << "[";
	for (int i = 0; i < _data.size(); i++)
	{
		if (_data[i]->type == "val")
		{
			if (i < _data.size() - 1)
			{
				cout << _data[i]->content << ",";
			}
			else
			{
				cout << _data[i]->content;
			}
		}
		else if (_data[i]->type == "list")
		{
			if (i < _data.size() - 1)
			{
				printlist(_data[i]->members);
				cout << ",";
			}
			else
			{
				printlist(_data[i]->members);
			}
		}
	}
	cout << "]";
}

void run(SyntaxTree* tree)
{
	SyntaxTree* nowtree = tree;
	while (nowtree != NULL && reg[9]->members.back()->content != "return" && reg[9]->members.back()->content != "break" && reg[9]->members.back()->content != "continue")
	{
		Data* members, * temp1, * temp2;
		switch (nowtree->nodekind)
		{
		case NameK:
			reg[0] = new Data;
			reg[0]->name = nowtree->val;
			reg[0]->type = "val";
			reg[0]->content = "0";
			reg[2] = new Data;
			reg[2]->content = "1";
			for (int i = 0; i < datas.size(); i++)
			{
				if (datas[i].back()->name == nowtree->val)
				{
					reg[0] = datas[i].back();
					reg[2]->content = "0";
					nameindex = i;
					break;
				}
			}
			if (reg[2]->content == "1")
			{
				vector<Data*> _data;
				_data.push_back(reg[0]);
				datas.push_back(_data);
				nameindex = datas.size() - 1;
			}
			break;
		case ListK:
			members = new Data;
			members->type = "list";
			for (int i = 0; i < nowtree->child.size(); i++)
			{
				run(nowtree->child[i]);
				members->members.push_back(reg[0]);
			}
			reg[0] = new Data;
			reg[0] = members;
			break;
		case ListIndexK:
			run(nowtree->child[0]);
			reg[5] = new Data;
			reg[5] = reg[0];
			run(nowtree->child[1]);
			reg[6] = reg[0];
			reg[0] = new Data;
			if (atoi(reg[6]->content.data()) < reg[5]->members.size())
			{
				reg[0] = reg[5]->members[atoi(reg[6]->content.data())];
			}
			else
			{
				error("数组越界", nowtree->code);
			}
			break;
		case AssignK:
			run(nowtree->child[0]);
			reg[1] = new Data;
			reg[1] = reg[0];
			run(nowtree->child[1]);
			if (reg[0]->type == "val")
			{
				reg[1]->type = "val";
				reg[1]->content = reg[0]->content;
			}
			else if (reg[0]->type == "list")
			{
				reg[1]->type = "list";
				reg[1]->members = reg[0]->members;
			}
			else if (reg[0]->type == "function")
			{
				reg[1]->type = "function";
				reg[1]->function = reg[0]->function;
			}
			break;
		case NumK:
		case StringK:
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = nowtree->val;
			break;
		case InputK:
			reg[0] = new Data;
			reg[0]->type = "val";
			cin >> reg[0]->content;
			break;
		case PrintK:
			for (int i = 0; i < nowtree->child.size(); i++)
			{
				run(nowtree->child[i]);
				if (reg[0]->type == "val")
				{
					cout << reg[0]->content;
				}
				else if (reg[0]->type == "list")
				{
					printlist(reg[0]->members);
				}
				else if (reg[0]->type == "function")
				{
					cout << "<function " << reg[0]->name << " at " << reg[0]->function << ">";
				}
			}
			break;
		case AddK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) + atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case SubK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) - atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case MulK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) * atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case DivK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) / atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case ModK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) % atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case EqK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) == atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case NeqK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) != atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case GtK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) > atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case GeqK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) >= atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case LtK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) < atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case LeqK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) <= atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case AndK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) && atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case OrK:
			temp1 = reg[3];
			temp2 = reg[4];
			run(nowtree->child[0]);
			reg[3] = new Data;
			reg[3] = reg[0];
			run(nowtree->child[1]);
			reg[4] = reg[0];
			reg[0] = new Data;
			reg[0]->type = "val";
			reg[0]->content = to_string(atoi(reg[3]->content.data()) || atoi(reg[4]->content.data()));
			reg[3] = temp1;
			reg[4] = temp2;
			break;
		case IfK:
			run(nowtree->child[0]);
			if (reg[0]->content == "1")
			{
				run(nowtree->child[1]);
			}
			else if (nowtree->child.size() > 2)
			{
				run(nowtree->child[2]);
			}
			break;
		case WhileK:
			run(nowtree->child[0]);
			while (reg[0]->content == "1")
			{
				run(nowtree->child[1]);
				if (reg[9]->members.back()->content == "break")
				{
					reg[9]->members.pop_back();
					break;
				}
				if (reg[9]->members.back()->content == "continue")
				{
					reg[9]->members.pop_back();
				}
				run(nowtree->child[0]);
			}
			break;
		case FuncDefK:
			run(nowtree->child[0]);
			reg[0]->type = "function";
			reg[0]->function = nowtree;
			break;
		case CallK:
			run(nowtree->child[0]);
			reg[7] = reg[0];
			if (nowtree->child.size() > 1 && nowtree->child[1] != NULL)
			{
				if (reg[7]->function->child.size() > 1 && (reg[7]->function->child[1] != NULL) && (reg[7]->function->child[1]->nodekind == ArgK) && (reg[7]->function->child[1]->child.size() == nowtree->child[1]->child.size()))
				{
					run(nowtree->child[1]);
				}
				else
				{
					error("函数参数数量错误", nowtree->code);
				}
			}
			else
			{
				if (reg[7]->function->child.size() > 1 && (reg[7]->function->child[1] != NULL) && reg[7]->function->child[1]->nodekind == ArgK && reg[7]->function->child[1]->child.size() != 0)
				{
					error("函数参数数量错误", nowtree->code);
				}
			}
			if (reg[7]->function->child.size() > 1)
			{
				if (reg[7]->function->child[1] != NULL && reg[7]->function->child[1]->nodekind == ArgK)
				{
					run(reg[7]->function->child[1]);
					if (reg[7]->function->child[2] != NULL && reg[7]->function->child.size() > 2)
					{
						run(reg[7]->function->child[2]);
						if (reg[9]->members.back()->content == "return")
						{
							reg[9]->members.pop_back();
						}
					}
					for (int i = 0; i < reg[7]->function->child[1]->child.size(); i++)
					{
						datas[atoi(reg[10]->members.back()->content.data())].pop_back();
						reg[10]->members.pop_back();
					}
				}
				else if (reg[7]->function->child[1] != NULL)
				{
					run(reg[7]->function->child[1]);
					if (reg[9]->members.back()->content == "return")
					{
						reg[9]->members.pop_back();
					}
				}
			}
			break;
		case CallArgK:
			for (int i = nowtree->child.size() - 1; i >= 0; i--)
			{
				run(nowtree->child[i]);
				args.push_back(reg[0]);
			}
			break;
		case ArgK:
			for (int i = 0; i < nowtree->child.size(); i++)
			{
				run(nowtree->child[i]);
				reg[8] = args.back();
				reg[8]->name = datas[nameindex].back()->name;
				datas[nameindex].push_back(reg[8]);
				reg[0] = new Data;
				reg[0]->content = to_string(nameindex);
				reg[10]->members.push_back(reg[0]);
				args.pop_back();
			}
			break;
		case ReturnK:
			if (nowtree->child.size() > 0)
			{
				run(nowtree->child[0]);
			}
			reg[2] = new Data;
			reg[2]->content = "return";
			reg[9]->members.push_back(reg[2]);
			break;
		case IncludeK:
			code = readFileIntoString(nowtree->child[0]->val.data());
			linepos = 0;
			reg[2] = new Data;
			reg[2]->function = tosyntaxtree();
			//printtree(reg[2]->function, 0);
			run(reg[2]->function);
			break;
		case BreakK:
			reg[2] = new Data;
			reg[2]->content = "break";
			reg[9]->members.push_back(reg[2]);
			break;
		case ContinueK:
			reg[2] = new Data;
			reg[2]->content = "continue";
			reg[9]->members.push_back(reg[2]);
			break;
		case IfdefK:
			reg[2] = new Data;
			reg[2]->content = "0";
			for (int i = 0; i < define.size(); i++)
			{
				if (define[i].name == nowtree->child[0]->val)
				{
					reg[2]->content = "1";
					break;
				}
			}
			if (reg[2]->content == "1")
			{
				definebool = 1;
				run(nowtree->child[1]);
			}
			else
			{
				definebool = 0;
			}
			break;
		case IfndefK:
			reg[2] = new Data;
			reg[2]->content = "1";
			for (int i = 0; i < define.size(); i++)
			{
				if (define[i].name == nowtree->child[0]->val)
				{
					reg[2]->content = "0";
					break;
				}
			}
			if (reg[2]->content == "1")
			{
				definebool = 1;
				run(nowtree->child[1]);
			}
			else
			{
				definebool = 0;
			}
			break;
		case ElifdefK:
			if (definebool == 0)
			{
				reg[2] = new Data;
				reg[2]->content = "0";
				for (int i = 0; i < define.size(); i++)
				{
					if (define[i].name == nowtree->child[0]->val)
					{
						reg[2]->content = "1";
						break;
					}
				}
				if (reg[2]->content == "1")
				{
					definebool = 1;
					run(nowtree->child[1]);
				}
				else
				{
					definebool = 0;
				}
			}
			break;
		case ElsedefK:
			if (definebool == 0)
			{
				run(nowtree->child[0]);
			}
			break;
		case EndifK:
			definebool = 0;
			break;
		default:
			break;
		}
		nowtree = nowtree->next;
	}
}

void shell()
{
	while (true)
	{
		readcode(">>> ");
		linepos = 0;
		SyntaxTree* syntaxtree = tosyntaxtree();
		//printtree(syntaxtree, 0);
		reg[10] = new Data;
		reg[9] = new Data;
		reg[2] = new Data;
		reg[2]->content = "start";
		reg[9]->members.push_back(reg[10]);
		run(syntaxtree);
	}
}

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		cout << "c-- [Version 1.0.0.1] on Windows\nFinished on Saturday,December 12th,2020\n";
		shell();
	}
	else
	{
		infile = fopen(argv[1], "r");
		ishell = false;
		linepos = 0;
		code = "";
		SyntaxTree* syntaxtree = tosyntaxtree();
		//printtree(syntaxtree, 0);
		reg[10] = new Data;
		reg[9] = new Data;
		reg[2] = new Data;
		reg[2]->content = "start";
		reg[9]->members.push_back(reg[10]);
		run(syntaxtree);
	}
}