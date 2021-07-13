#pragma once

#include <stdio.h>
#include <string.h>

#define MAX_DEFINE_LEN 1024

struct _DefineList
{
	char* define_name[MAX_DEFINE_LEN];	//存储宏的名称
	char* define_content[MAX_DEFINE_LEN];	//存储宏的内容
	int define_len;	//宏的数量
	struct _DefineList* next;
};

struct DefineList
{
	struct _DefineList* nowdefinelist;	//当前宏列表
	struct _DefineList* alldefinelist;
};

char ch;
static char* include_path = ".include\\";	//包含的默认目录
static int defineflag = 0;
static struct DefineList* definelist = NULL;

char* preprocessor(FILE* file);
char* dealprecode(struct DynString* code, FILE* file,struct DynString* precode);
char* getnextprediv(struct DynString* precode, int* pos);
int search_define(char c);
int search_define2(char* name);
struct DefineList* definelist_init();
struct _DefineList* _definelist_init();
void definelist_add(char* name,char* content);
void definelist_findempty();
void definelist_del(char* name);