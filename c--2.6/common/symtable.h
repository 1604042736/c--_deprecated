#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include "filecontext.h"
#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_VAR_NUM 1000
#define MAX_TYPE_NUM 1000

typedef struct _ti {
    char *name;
    struct AST *bit_field;
    struct Type *type;
    struct _ti *same_hash;  //把hash值相同的表项串起来
    struct SymTable *table; //所属的符号表
    FileContext context;
} TableItem;

//每个作用域对应一个符号表
typedef struct SymTable {
    TableItem *vars[MAX_VAR_NUM];
    TableItem *types[MAX_TYPE_NUM]; //存储一些自定义类型
    struct SymTable *parent;        //父作用域
    struct SymTable *child;         //子作用域链表头
    struct SymTable *pre, *next;
} SymTable;

TableItem *tableitem_new();
void tableitem_init(TableItem *, char *name, struct Type *type);

SymTable *symtable_new();
void symtable_init(SymTable *);
void symtable_add_var(SymTable *, TableItem *);
void symtable_add_var_type(SymTable *, struct Type *type);
TableItem *symtable_find_type(SymTable *, char *name);
TableItem *symtable_find_var(SymTable *, char *name);
SymTable *symtable_enterscope(SymTable *);
SymTable *symtable_leavescope(SymTable *);
void symtable_print(SymTable *, int indent);

#ifdef __cplusplus
}
#endif

#endif