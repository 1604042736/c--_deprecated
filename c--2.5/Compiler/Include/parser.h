#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "ast.h"
#include "type.h"

struct Parser
{
	struct Lexer* lexer;
};

struct Parser* parser_new(struct Lexer* lexer);
void parser_match(struct Parser*, enum TokenType);
void parser_error(struct Parser*, char*, ...);
struct AST* parser_start(struct Parser*);
struct Type* parser_type(struct Parser*);
struct AST* parser_statements(struct Parser*);
struct AST* parser_statement(struct Parser*);
struct AST* parser_compoundstmt(struct Parser*);
struct AST* parser_ifstmt(struct Parser*);
struct AST* parser_exp(struct Parser*);
struct AST* parser_exp_assign(struct Parser*);
struct AST* parser_exp_or(struct Parser*);
struct AST* parser_exp_and(struct Parser*);
struct AST* parser_exp_bitor(struct Parser*);
struct AST* parser_exp_bitxor(struct Parser*);
struct AST* parser_exp_bitand(struct Parser*);
struct AST* parser_exp_compare(struct Parser*);
struct AST* parser_exp_shift(struct Parser*);
struct AST* parser_exp_sum(struct Parser*);
struct AST* parser_exp_term(struct Parser*);
struct AST* parser_exp_unary(struct Parser*);
struct AST* parser_exp_primary(struct Parser*);
struct AST* parser_exp_atom(struct Parser*);
struct AST* parser_defstmt(struct Parser*);
struct AST* parser_funcdef(struct Parser*);
struct AST* parser_argsdef(struct Parser*);
struct AST* parser_returnstmt(struct Parser*);
struct AST* parser_breakstmt(struct Parser*);
struct AST* parser_continuestmt(struct Parser*);
struct AST* parser_externstmt(struct Parser*);
struct AST* parser_whilestmt(struct Parser*);
struct AST* parser_structdef(struct Parser*);