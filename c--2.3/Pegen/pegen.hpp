#pragma once

#include <string>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

/*
Pegen解析器
*/
class Pegen
{
public:
    Pegen(char *);
    ofstream out;
    string outfilename;
    string parsername = "GrammarParser";
    map<string, void *> rules_def; // 规则定义
    map<string, void *> loop_def;  //循环规则定义
    map<string, int> type_def;     //类型定义
    int loop_count = 0;
    int type_count = 0;
    bool assign = true;     //是否赋值(NameLeaf)
    vector<string> defines; //变量声明
};

Pegen::Pegen(char *outfilename)
{
    this->outfilename = outfilename;
    this->out.open(outfilename, ios::out);
}

Pegen *pegen;