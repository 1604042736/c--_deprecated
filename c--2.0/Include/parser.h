#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "globals.h"
#include "symbol.h"

struct Parser
{
	struct Symbol* symbol;
	struct Lexer* lexer;
};

struct Parser* parser_init(struct Lexer* lexer);
struct SyntaxTree* parser_parse(struct Parser* parser);
struct SyntaxTree* parser_body(struct Parser* parser,char* indent);
struct SyntaxTree* parser_sentence(struct Parser* parser);
struct SyntaxTree* parser_if(struct Parser* parser);
struct SyntaxTree* parser_exp(struct Parser* parser);
struct SyntaxTree* parser_exp_or(struct Parser* parser);
struct SyntaxTree* parser_exp_and(struct Parser* parser);
struct SyntaxTree* parser_exp_compare(struct Parser* parser);
struct SyntaxTree* parser_exp_shift(struct Parser* parser);
struct SyntaxTree* parser_exp_add(struct Parser* parser);
struct SyntaxTree* parser_exp_mul(struct Parser* parser);
struct SyntaxTree* parser_exp_primary(struct Parser* parser);
struct SyntaxTree* parser_args(struct Parser* parser);
struct SyntaxTree* parser_assign(struct Parser* parser);
struct SyntaxTree* parser_while(struct Parser* parser);
struct SyntaxTree* parser_break(struct Parser* parser);
struct SyntaxTree* parser_continue(struct Parser* parser);
struct SyntaxTree* parser_return(struct Parser* parser);
struct SyntaxTree* parser_int(struct Parser* parser,int exp);
struct SyntaxTree* parser_char(struct Parser* parser, int exp);
struct SyntaxTree* parser_void(struct Parser* parser, int exp);
struct SyntaxTree* parser_struct(struct Parser* parser);
struct SyntaxTree* parser_define(struct Parser* parser, int exp, enum Type type, char* other_type);
void parser_match(struct Parser* parser, enum Token expect);