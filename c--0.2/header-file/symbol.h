#pragma once

#include "globals.h"
#include "error.h"

#define SHIFT 4
#define SIZE 211

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
	int sc;
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

void st_init(Symtab* symtab);
static int hash(char* key);
void st_insert(Symtab* symtab, char* name, int lineno, int loc);
int st_lookup(Symtab* symtab, char* name);
void st_lookup_point(Symtab* symtab, char* name,int way);
void printSymTab(Symtab* symtab);
void analyzer_init(Analyzer* analyzer, Symtab* symtab) ;
static void traverse(Analyzer* analyzer,TreeNode* t, void(*preProc)(Analyzer*,TreeNode*), void(*postProc)(Analyzer*,TreeNode*));
static void nullProc(Analyzer* analyzer, TreeNode* t);
static void insertNode(Analyzer* analyzer,TreeNode* t);
void buildSymtab(Analyzer* analyzer, TreeNode* syntaxTree) ;
Analyzer* symbol_start(TreeNode* syntaxTree,char* filename);
