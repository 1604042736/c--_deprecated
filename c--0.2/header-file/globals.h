#pragma once

#include<fstream>
#include<cstdlib>
#include<vector>
#include<iostream>
#include<stack>
using namespace std;

#define MAXCHILD 4

enum NodeKind
{
	Ifk,WhileK,ForK,DoWhileK,IntK,VoidK,Opk,Idk,Numk,AssignK,ParamK,BreaKK,ContinueK,CallK,ReturnK,PrintK,InputK,ArrayNumK,StructK,MemberK,
	ArrayAssignK,ArrayK,NewArrayK,DataKindK,FuncStateK,FuncComeK,UserVarK,ConstStringK,StringK,PointerK,PointerAssignK,NewPointerK,
};

typedef struct TreeNode
{
	TreeNode* child[MAXCHILD];
	TreeNode* sibling;
	NodeKind kind;
	char* name;
	int val;
	int lineno;
	int linepos;
}TreeNode;

char getCh(FILE* file);
void ungetCh(char ch,FILE* file);
void printTree(TreeNode* t,int s);
