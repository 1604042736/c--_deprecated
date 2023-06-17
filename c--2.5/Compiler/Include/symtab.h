#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "type.h"

#define HASHTAB_EXPAND 1024
#define SCOPE_EXPAND 5

#define SF_ARG 1
#define SF_CONST 1<<1
#define SF_EXTERN 1<<2
#define SF_STRUCT 1<<3
#define SF_MEMBER 1<<4

#define CHECK_FLAG(a,b) (((a)&(b))==(b))

struct Symbol;
//符号表
struct Symtab
{
	int scope_len;	//作用域长度
	int scope_size;	//scope总长度
	int *scope;	//作用域
	int scope_count;	//作用域计数

	int symbol_size;	//symbols总长度
	int free_symbol_num;	//空闲数量
	struct Symbol** symbols;	//所有符号

	struct Symbol* curfunc;	//当前函数
	struct Symbol* curstruct;	//当前结构体

	int globeflag;
};

//符号
struct Symbol
{
	int flag;	//标志
	char* name;	//符号名
	char* anname;	//别称
	int* scope;	//作用域
	int scope_size;
	struct Type* type;	//符号类型

	struct //变量符号属性
	{
		int offset;	//偏移
	}var;

	struct //常量符号属性
	{
		char* dataname;	//数据名称
	}cons;

	struct //函数符号属性
	{
		struct Symbol** args;	//参数
		int arg_num;

		int scopeesp;	//作用域大小
		int curesp;	//当前栈指针
		int argoff;	//参数偏移
	}func;

	struct //结构体符号属性
	{
		struct Symbol** members;	//成员
		int member_num;

		int size;	//总长度
	}struc;

	struct Symbol* nextsame;	//同名符号
};

struct Symtab* symtab_new();
void symtab_addsymbol(struct Symtab*,struct Symbol*);
void symtab_enterscope(struct Symtab*);
void symtab_leavescope(struct Symtab*);
struct Symbol* symtab_lookup(struct Symtab*, char*,int*,int);
void symtab_print(struct Symtab*);
struct Symbol* symbol_new(struct Symtab*,char*,struct Type*,int);
void symtab_typecheck(struct Symtab*, struct AST*);

int hash_string(char*);