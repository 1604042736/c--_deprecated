#pragma once

#include "parser.h"
#include "codeobject.h"
#include "listobject.h"
#include "stringobject.h"
#include "intobject.h"
#include "opcode.h"

static map<string, int>operatormap = { {"+",ADD} ,{"-",SUB},{"*",MUL},{"/",DIV},{"%",MOD} ,{"==",EQ},{"or",OR} ,{">",GT},
	{">=",GEQ},{"<",LT}, {"<=",LEQ},{"!=",NEQ},{"and",AND} };
void generator(Object*,SyntaxTree*);
bool searchname(vector<Object*>,string);	//判断一个名字是否出现过