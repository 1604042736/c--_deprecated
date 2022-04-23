#pragma once

#include <vector>
#include <string>
#include <iostream>
using namespace std;

/*
语法树
*/
class AST
{
public:
    virtual void print(int space = 0) {}         //输出
    virtual void gen(){};                        //转换成对应的代码
    virtual string get_type() { return "void"; } //获取类型
    void print_space(int);
    string toString() { return ""; }
};

void AST::print_space(int space)
{
    for (int i = 0; i < space; i++)
    {
        cout << "  ";
    }
}