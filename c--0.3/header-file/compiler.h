#pragma once

#include "globals.h"
#include "error.h"

#define MAXTOKENSTRING 1024
#define MAXTOKENSTR 37

enum TokenType
{
	FL_EOF,
	TK_ADD,TK_SUB,TK_MUL,TK_DIV,TK_MOD,
	TK_ASSIGN,TK_EQ,TK_NEQ,TK_GT,TK_GEQ,TK_LT,TK_LEQ,TK_AND,TK_OR,
	TK_LLITTLE,TK_RLITTLE,TK_LMIDDLE,TK_RMIDDLE,TK_LLARGE,TK_RLARGE,
	TK_SEIM,TK_COMMA,TK_POINT,
	TK_ID,TK_NUM,TK_CONSTSTRING,
	KW_IF,KW_ELSE,KW_FOR,KW_WHILE,KW_DO,KW_RETURN,KW_BREAK,KW_CONTINUE,KW_INT,KW_VOID,KW_STRUCT,KW_STRING,
	KW_PRINT,KW_INPUT
};

static struct TokenStr
{
	char* str;
	TokenType tk;
}tokenstr[MAXTOKENSTR]=
{
	{(char*)"if",KW_IF},{(char*)"else",KW_ELSE},{(char*)"for",KW_FOR},{(char*)"while",KW_WHILE},{(char*)"do",KW_DO},
	{(char*)"return",KW_RETURN},{(char*)"break",KW_BREAK},{(char*)"continue",KW_CONTINUE},{(char*)"int",KW_INT},{(char*)"void",KW_VOID},
	{(char*)"print",KW_PRINT},{(char*)"input",KW_INPUT},{(char*)"struct",KW_STRUCT},{(char*)"string",KW_STRING},
	{(char*)"+",TK_ADD},{(char*)"-",TK_SUB},{(char*)"*",TK_MUL},{(char*)"/",TK_DIV},{(char*)"%",TK_MOD},{(char*)"=",TK_ASSIGN},
	{(char*)"==",TK_EQ},{(char*)"!=",TK_NEQ},{(char*)">",TK_GT},{(char*)">=",TK_GEQ},{(char*)"<",TK_LT},{(char*)"<=",TK_LEQ},
	{(char*)"&&",TK_AND},{(char*)"||",TK_OR},{(char*)"(",TK_LLITTLE},{(char*)")",TK_RLITTLE},{(char*)"[",TK_LMIDDLE},
	{(char*)"]",TK_RMIDDLE},{(char*)"{",TK_LLARGE},{(char*)"}",TK_RLARGE},{(char*)";",TK_SEIM},{(char*)",",TK_COMMA},{(char*)".",TK_POINT}
};

TreeNode* compiler(char* f,vector<char> filei);
void gettoken();
int is_nodigit(char c);
int is_digit(char c);
TokenType getId();
void getNum();
TreeNode* sentence();
TreeNode* int_stmt();
void match(TokenType t);
TreeNode* parameter();
TreeNode* exp();
TreeNode* exp_two();
TreeNode* exp_three();
TreeNode* exp_four();
TreeNode* exp_five();
TreeNode* exp_six();
TreeNode* block();
TreeNode* return_stmt();
TreeNode* if_stmt();
TreeNode* for_stmt();
TreeNode* while_stmt();
TreeNode* do_stmt();
TreeNode* break_stmt();
TreeNode* continue_stmt();
TreeNode* assign_stmt();
TreeNode* void_stmt();
char* getstr(TokenType t);
TreeNode* newNode(NodeKind kind);
TreeNode* print_stmt();
TreeNode* input_stmt();
TreeNode* struct_stmt();
void getString();
TreeNode* string_stmt();
char* copyString(char* s);
