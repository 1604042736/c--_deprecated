#pragma once

#include "symbol.h"
#include "compiler.h"
#include "high.h"
#include "error.h"

typedef struct
{
	vector<int> way;
	int num;
}params_num;

char* run(TreeNode* tree,Analyzer* analyzer,char* name);
char* cGen(TreeNode* tree);
char* get_exp(TreeNode* tree);
char* inttochar(int n);
