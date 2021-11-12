#pragma once

#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<cstdlib>

#include "lexer.h"
#include "globals.h"
using namespace std;

enum NodeType	//�ڵ�����
{
	NT_NAME,NT_NUM,NT_STRING,
	NT_PROGRAM,
	NT_CONDITION,NT_OPERATION,NT_LOOP,NT_ASSIGN,NT_FUNCTIONDEF,NT_SUBSCRIPT,NT_CALL,NT_BREAK,NT_CONTINUE,NT_RETURN,NT_LIST,
	NT_ATTRIBUTE,
	NT_NULL
};

enum ASTType	//�﷨������
{
	AT_SYNTAXTREE,	//�﷨��
	AT_LIST,	//�б�
	AT_STRING,	//�ַ���
	AT_INT	//����
};

struct SyntaxTree	//�﷨��
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

SyntaxTree* syntaxtree_init(NodeType, ASTType, int, int);	//��ʼ���﷨��
void syntaxtree_print(SyntaxTree*, int);

Parser* parser_init(Lexer*,BlockStyle);
SyntaxTree* parser_parse(Parser*);
void parser_match(Parser*, Token);
SyntaxTree* parser_body(Parser*,string,Token);
SyntaxTree* parser_sentence(Parser*);
SyntaxTree* parser_if(Parser*);	//��ͬ�Ĵ�����ʽ
SyntaxTree* parser_exp(Parser*);
SyntaxTree* parser_exp_or(Parser*);
SyntaxTree* parser_exp_and(Parser*);
SyntaxTree* parser_exp_compare(Parser*);
SyntaxTree* parser_exp_shift(Parser*);
SyntaxTree* parser_exp_add(Parser*);
SyntaxTree* parser_exp_mul(Parser*);
SyntaxTree* parser_exp_primary(Parser*);
SyntaxTree* parser_while(Parser*);	//��ͬ�Ĵ�����ʽ
SyntaxTree* parser_assign(Parser*);
SyntaxTree* parser_def(Parser*);	//��ͬ�Ĵ�����ʽ
SyntaxTree* parser_args(Parser*);
SyntaxTree* parser_break(Parser*);
SyntaxTree* parser_continue(Parser*);
SyntaxTree* parser_return(Parser*);