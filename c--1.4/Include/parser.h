#pragma once

#include "lexer.h"
#include "astobject.h"
#include "doubleobject.h"
#include "preprocessor.h"
#include "dictobject.h"
#include "stringobject.h"

enum BlockStyle
{
	BRACES,INDENT,BEGINEND,ENDNAME,END
};

struct Parser
{
	struct Lexer* lexer;
	enum BlockStyle blockstyle;
	int importmode;
	struct DictObject* namespace_names;	//可以用符号表
};

struct Parser* Parser_New(struct Lexer*);
struct Object* Parser_Parse(struct Parser*);
struct Object* Parser_body(struct Parser*,char*,int);
struct Object* Parser_sentence(struct Parser*);
void Parser_match(struct Parser*, int);
struct Object* Parser_if(struct Parser*);
struct Object* Parser_exp_logical_or(struct Parser*);
struct Object* Parser_exp_logical_and(struct Parser*);
struct Object* Parser_exp_equality(struct Parser*);
struct Object* Parser_exp_additive(struct Parser*);
struct Object* Parser_exp_multiplicative(struct Parser*);
struct Object* Parser_exp_postfix(struct Parser*);
struct Object* Parser_exp_primary(struct Parser*);
struct Object* Parser_exp(struct Parser*);
struct Object* Parser_assignment(struct Parser*);
struct Object* Parser_exp_argument(struct Parser*);
struct Object* Parser_jump(struct Parser*);
struct Object* Parser_functiondef(struct Parser*);
struct Object* Parser_while(struct Parser*);
void Parser_preprocess(struct Parser*);
struct Object* Parser_import(struct Parser*);
struct Object* Parser_class(struct Parser*);
struct Object* Parser_try(struct Parser*);