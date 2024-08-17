#ifndef __TYPECHECKER_H__
#define __TYPECHECKER_H__

#include "ast.h"
#include "symtable.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    SymTable *symtab;
    SymTable *cur_symtab;
} TypeChecker;

TypeChecker *typechecker_new();
void typechecker_init(TypeChecker *, SymTable *);
void typechecker_check(TypeChecker *, AST *);

#ifdef __cplusplus
}
#endif

#endif