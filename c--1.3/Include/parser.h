#pragma once

#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<cstdlib>

#include "lexer.h"
#include "globals.h"
using namespace std;

enum NodeType	//节点类型
{
	NT_NAME,NT_NUM,NT_STRING,
	NT_PROGRAM,
	NT_CONDITION,NT_OPERATION,NT_LOOP,NT_ASSIGN,NT_FUNCTIONDEF,NT_SUBSCRIPT,NT_CALL,NT_BREAK,NT_CONTINUE,NT_RETURN,NT_LIST,
	NT_ATTRIBUTE,
	NT_NULL
};

enum ASTType	//语法树类型
{
	AT_SYNTAXTREE,	//语法树
	AT_LIST,	//列表
	AT_STRING,	//字符串
	AT_INT	//数字
};

struct SyntaxTree	//语法树
{
	NodeType nodetype;
	ASTType asttype;
	map<string, SyntaxTree*> child;
	int lineno, linepos;

	int value;
	string str;
	vector<SyntaxTree*> list;
};

struct Parser
{
	Lexer* lexer;
	BlockStyle blockstyle;
};

SyntaxTree* syntaxtree_init(NodeType, ASTType, int, int);	//初始化语法树
void syntaxtree_print(SyntaxTree*, int);

Parser* parser_init(Lexer*,BlockStyle);
SyntaxTree* parser_parse(Parser*);
void parser_match(Parser*, Token);
SyntaxTree* parser_body(Parser*,string,Token);
SyntaxTree* parser_sentence(Parser*);
SyntaxTree* parser_if(Parser*);	//不同的代码样式
SyntaxTree* parser_exp(Parser*);
SyntaxTree* parser_exp_or(Parser*);
SyntaxTree* parser_exp_and(Parser*);
SyntaxTree* parser_exp_compare(Parser*);
SyntaxTree* parser_exp_shift(Parser*);
SyntaxTree* parser_exp_add(Parser*);
SyntaxTree* parser_exp_mul(Parser*);
SyntaxTree* parser_exp_primary(Parser*);
SyntaxTree* parser_while(Parser*);	//不同的代码样式
SyntaxTree* parser_assign(Parser*);
SyntaxTree* parser_def(Parser*);	//不同的代码样式
SyntaxTree* parser_args(Parser*);
SyntaxTree* parser_break(Parser*);
SyntaxTree* parser_continue(Parser*);
SyntaxTree* parser_return(Parser*);