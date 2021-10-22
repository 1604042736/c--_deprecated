#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include<map>
#include<cstdio>

#include "globals.h"
using namespace std;

#define isName(c)	(((c>='a'&&c<='z')||(c>='A'&&c<='Z'))||c=='_')

struct Preprocessor
{
	BlockStyle blockstyle;
};

static map<string, string> define;
static string include_path = ".include";
static bool defineflag = false;

Preprocessor* preprocessor_init();
string preprocessor(Preprocessor*,string);	//预处理
string dealprecode(Preprocessor* ,string&, string,int&, string);	//处理预处理指令
string getnextprediv(string, int&);	//获取下一个指令
string search_define(char);