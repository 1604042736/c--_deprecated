#pragma once

#include <stdlib.h>
#include <stdio.h>

#define MAXSYNTAXTREELISTLEN 1024

/*���ִ����ķ�ʽ*/
enum BlockStyle
{
	INDENT,	//����
	BEGINEND,	//begin end
	BRACES	//������
};

enum BlockStyle blockstyle;

/*�﷨���ڵ����*/
enum NodeParam
{
	NP_TARGET,NP_EXP, NP_ID, NP_ARGS,NP_BODY,NP_LEFT,NP_RIGHT,NP_VALUE,NP_ELSES,NP_ATTR, NP_DEFAULT,
	NP_LEN	//��ʶ����
};

/*�﷨���ڵ�����*/
enum NodeType
{
	NT_PROGRAM,NT_IF,NT_WHILE,NT_NAME,NT_NUM,NT_ADD,NT_SUB,NT_MUL,NT_DIV,NT_MOD,NT_AND,NT_OR,NT_ASSIGN,NT_GT,NT_GEQ,NT_LT,
	NT_LEQ,NT_EQ,NT_NEQ,NT_LSHIFT,NT_RSHIFT,NT_CALL,NT_CHAR,NT_STRING,NT_INDEX,NT_BREAK,NT_CONTINUE,NT_RETURN,NT_ATTR,NT_DEFINE,
	NT_FUNCTION,
	NT_NULL	//�﷨�����Ͳ���AT_SYNTAXTREE���﷨���ڵ�����
};

/*�﷨������*/
enum ASTType
{
	AT_SYNTAXTREE,	//�﷨��
	AT_LIST,	//�б�
	AT_STRING,	//�ַ���
	AT_INT	//����
};

/*�﷨��*/
struct SyntaxTree
{
	struct SyntaxTree* child[NP_LEN];
	enum NodeType nodetype;
	enum ASTType asttype;
	int lineno, linepos;

	int value;
	char* string;
	struct SyntaxTreeList* list;
};

/*�﷨���б�,�洢����﷨��*/
struct _SyntaxTreeList
{
	int nowpos;	//����λ��
	struct SyntaxTree* syntaxtree[MAXSYNTAXTREELISTLEN];
	struct _SyntaxTreeList* next;
};

struct SyntaxTreeList
{
	struct _SyntaxTreeList* nowtreelist;
	struct _SyntaxTreeList* alltreelist;
	int len;
};

struct SyntaxTree* syntaxtree_init(enum NodeType nodetype, enum ASTType asttype,int lineno, int linepos);
struct _SyntaxTreeList* _syntaxtreelist_init();
struct SyntaxTreeList* syntaxtreelist_init();
/*���� Define_List*/
void syntaxtreelist_add(struct SyntaxTreeList *syntaxtreelist,struct SyntaxTree* syntaxtree);
void syntaxtreelist_findempty(struct SyntaxTreeList* syntaxtreelist);
struct SyntaxTree* syntaxtreelist_index(struct SyntaxTreeList* syntaxtreelist,int index);
void syntaxtree_print(struct SyntaxTree* syntaxtree, int space);