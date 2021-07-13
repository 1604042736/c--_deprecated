#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"

#define MAXSYMBOLLEN 1024

/*����*/
enum Type
{
	T_INT,	//����
	T_CHAR,	//�ַ�
	T_VOID,
	T_STRING,	//�ַ���
	T_FUNCTION,	//����
	T_ARRAY,	//����
	T_STRUCT	//�ṹ��
};

struct SymbolItem
{
	char* name;	//����
	enum Type type;	//����
	char* return_type;	//��������
	char* struct_type;	//�ṹ������
	char* array_type;	//��������
	struct SyntaxTree* args;	//��������
};

/*��Ȼ���� DefineList �ķ���*/
struct _Symbol
{
	int nowpos;
	struct SymbolItem* symbolitem[MAXSYMBOLLEN];
	struct _Symbol* next;
};

struct Symbol
{
	struct _Symbol* nowsymbol;
	struct _Symbol* allsymbol;
};

struct SymbolItem* symbolitem_init(char* name, enum Type type);
struct _Symbol* _symbol_init();
struct Symbol* symbol_init();
struct SymbolItem* symbol_search(struct Symbol* symbol, char* name);
void symbol_findempty(struct Symbol* symbol);
void symbol_add(struct Symbol* symbol, struct SymbolItem* symbolitem);
void symbol_print(struct Symbol* symbol);
struct SymbolItem* symbol_addval(struct Symbol* symbol, char* name, enum Type type);
void symbol_addfunc(struct Symbol* symbol, char* name, char* return_type,struct SyntaxTree* args);
void symbol_addarray(struct Symbol* symbol, char* name, char* array_type);
void symbol_addstruct(struct Symbol* symbol, char* name, char* struct_type);