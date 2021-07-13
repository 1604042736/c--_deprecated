#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"

#define MAXSYMBOLLEN 1024

/*类型*/
enum Type
{
	T_INT,	//整型
	T_CHAR,	//字符
	T_VOID,
	T_STRING,	//字符串
	T_FUNCTION,	//函数
	T_ARRAY,	//数组
	T_STRUCT	//结构体
};

struct SymbolItem
{
	char* name;	//名称
	enum Type type;	//类型
	char* return_type;	//返回类型
	char* struct_type;	//结构体类型
	char* array_type;	//数组类型
	struct SyntaxTree* args;	//函数参数
};

/*仍然采用 DefineList 的方法*/
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