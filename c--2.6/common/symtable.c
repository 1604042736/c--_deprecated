#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringutil.h"
#include "symtable.h"
#include "type.h"

TableItem *tableitem_new() { return (TableItem *)malloc(sizeof(TableItem)); }

void tableitem_init(TableItem *self, char *name, struct Type *type)
{
    memset(self, 0, sizeof(TableItem));
    self->name = name;
    self->type = type;
}

SymTable *symtable_new() { return (SymTable *)malloc(sizeof(SymTable)); }

void symtable_init(SymTable *self)
{
    memset(self->vars, 0, sizeof(self->vars));
    memset(self->types, 0, sizeof(self->types));
    self->parent = self->child = self->pre = self->next = NULL;
}

void symtable_add_var(SymTable *self, TableItem *item)
{
    unsigned long long hash = string_hash(item->name);
    unsigned long long k = hash % MAX_VAR_NUM;
    if (self->vars[k] == NULL)
        self->vars[k] = item;
    else //冲突
    {
        TableItem *p = self->vars[k];
        while (p->same_hash != NULL) p = p->same_hash;
        p->same_hash = item;
    }
    item->table = self;
}

void symtable_add_var_type(SymTable *self, Type *type)
{
    TableItem *typeitem = tableitem_new();
    tableitem_init(typeitem, type->name, type);

    unsigned long long hash = string_hash(typeitem->name);
    unsigned long long k = hash % MAX_TYPE_NUM;
    if (self->types[k] == NULL)
        self->types[k] = typeitem;
    else //冲突
    {
        TableItem *p = self->types[k];
        while (p->same_hash != NULL) p = p->same_hash;
        p->same_hash = typeitem;
    }
    typeitem->table = self;
}

TableItem *symtable_find_type(SymTable *self, char *name)
{
    unsigned long long hash = string_hash(name);
    unsigned long long k = hash % MAX_TYPE_NUM;
    TableItem *p = self->types[k];
    while (p != NULL)
    {
        if (!strcmp(p->name, name)) break;
        p = p->same_hash;
    }
    if (p == NULL && self->parent != NULL)
        return symtable_find_type(self->parent, name);
    return p;
}

TableItem *symtable_find_var(SymTable *self, char *name)
{
    unsigned long long hash = string_hash(name);
    unsigned long long k = hash % MAX_VAR_NUM;
    TableItem *p = self->vars[k];
    while (p != NULL)
    {
        if (!strcmp(p->name, name)) break;
        p = p->same_hash;
    }
    if (p == NULL && self->parent != NULL)
        return symtable_find_var(self->parent, name);
    return p;
}

/*
进入一个新的作用域并返回那个作用域的符号表
*/
SymTable *symtable_enterscope(SymTable *self)
{
    SymTable *symtab = symtable_new();
    symtable_init(symtab);
    if (self->child == NULL)
        self->child = symtab;
    else
    {
        SymTable *p = self->child;
        while (p->next != NULL) p = p->next;
        p->next = symtab;
        symtab->pre = p;
    }
    symtab->parent = self;
    return symtab;
}

/*
返回上层作用域的符号表
*/
SymTable *symtable_leavescope(SymTable *self) { return self->parent; }

#define PRINT_INDENT                                                           \
    do                                                                         \
    {                                                                          \
        for (int i = 0; i < indent; i++) printf("    ");                       \
    } while (0)
void symtable_print(SymTable *self, int indent)
{
    PRINT_INDENT;
    printf("Symbol Table: %p\n", self);
    PRINT_INDENT;
    printf("%-*s%-*s\n", 20, "Name", 20, "Type");
    for (int i = 0; i < MAX_VAR_NUM; i++)
    {
        if (self->vars[i] == NULL) continue;
        PRINT_INDENT;
        printf("%-*s%-*s\n", 20, self->vars[i]->name, 20,
               type_tostr(self->vars[i]->type));
    }
    for (int i = 0; i < MAX_TYPE_NUM; i++)
    {
        if (self->types[i] == NULL) continue;
        PRINT_INDENT;
        printf("%-*s%-*s\n", 20, self->types[i]->name, 20,
               type_tostr(self->types[i]->type));
    }
    SymTable *p = self->child;
    while (p != NULL)
    {
        symtable_print(p, indent + 1);
        p = p->next;
    }
}