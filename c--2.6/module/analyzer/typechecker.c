#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "stringutil.h"
#include "typechecker.h"

TypeChecker *typechecker_new()
{
    return (TypeChecker *)malloc(sizeof(TypeChecker));
}

void typechecker_init(TypeChecker *self, SymTable *symtab)
{
    if (symtab == NULL)
    {
        symtab = symtable_new();
        symtable_init(symtab);
    }
    self->cur_symtab = self->symtab = symtab;
}

void typechecker_check_brother(TypeChecker *self, AST *ast)
{
    AST *p = ast;
    while (p != NULL)
    {
        typechecker_check(self, p);
        p = p->sibling;
    }
}

void typechecker_check_child(TypeChecker *self, AST *ast)
{
    for (int i = 0; i < MAX_AST_CHILD_NUM; i++)
    {
        typechecker_check(self, ast->child[i]);
        if (ast->child[i] != NULL)
            typechecker_check_brother(self, ast->child[i]->sibling);
    }
}

#define COMPLETE(t)                                                            \
    do                                                                         \
    {                                                                          \
        Type *_type = t;                                                       \
        t = typechecker_complete_type(self, t);                                \
        if (t == NULL)                                                         \
        {                                                                      \
            t = _type;                                                         \
            return NULL;                                                       \
        }                                                                      \
    } while (0)
/*
补全类型, 并返回补全后的类型
*/
Type *typechecker_complete_type(TypeChecker *self, Type *type)
{
    if (type == NULL) return type;
    switch (type->kind)
    {
    case T_POINTER: COMPLETE(type->t.pointee_type); break;
    case T_ARRAY: COMPLETE(type->t.array.element_type); break;
    case T_FUNCTION:
        COMPLETE(type->t.function.return_type);
        {
            ParamAttribute *t = type->t.function.param_types;
            while (t != NULL)
            {
                COMPLETE(t->type);
                t = t->next;
            }
        }
        break;
    case T_ENUM:
    case T_UNION:
    case T_STRUCT:
    case T_TYPEDEF:
        TableItem *item = symtable_find_type(self->cur_symtab, type->name);
        while (item != NULL
               && item->type->kind != type->kind) // typedef名和记录名相同
            item = item->same_hash;
        if (item == NULL || item->type->kind != type->kind) return NULL;
        type = item->type;
        break;
    }
    return type;
}

#define NEWVAR(name, type, ast)                                                \
    do                                                                         \
    {                                                                          \
        tabitem = tableitem_new();                                             \
        tableitem_init(tabitem, name, type);                                   \
        FILECONTEXT_FROMASTCONTEXT(context, ast->context);                     \
        tabitem->context = context;                                            \
        symtable_add_var(self->cur_symtab, tabitem);                           \
    } while (0)

#define NEWTYPE(var, type)                                                     \
    do                                                                         \
    {                                                                          \
        var = type_new();                                                      \
        type_init(var, type);                                                  \
    } while (0)

#define NEWAST(var, kind, context)                                             \
    do                                                                         \
    {                                                                          \
        var = ast_new();                                                       \
        ast_init(var, kind, context);                                          \
    } while (0)

void typechecker_check(TypeChecker *self, AST *ast)
{
    if (ast == NULL) return;
    char *name;
    Type *type;
    TableItem *tabitem;
    AST *p;
    EnumAttribute *e;
    FileContext context;
    if (ast->type != NULL)
    {
        type = ast->type;
        ast->type = typechecker_complete_type(self, ast->type);
        if (ast->type == NULL)
            errora(ast->context, "incomplete type: '%s'", type_tostr(type));
    }
    switch (ast->kind)
    {
    case AK_FOR:
        self->cur_symtab = symtable_enterscope(self->cur_symtab);
        typechecker_check_child(self, ast);
        self->cur_symtab = symtable_leavescope(self->cur_symtab);
        break;
    case AK_FUNCTIONDEF:
        NEWVAR(ast->val, ast->type, ast);

        if (ast->funcdef_body != NULL) // ==NULL是声明
        {
            self->cur_symtab = symtable_enterscope(self->cur_symtab);
            typechecker_check_brother(self, ast->funcdef_param);
            typechecker_check_brother(
                self,
                ast->funcdef_body
                    ->compound_stmt); //函数参数和函数体属于同一个作用域
            self->cur_symtab = symtable_leavescope(self->cur_symtab);
        }
        break;
    case AK_COMPOUND:
        self->cur_symtab = symtable_enterscope(self->cur_symtab);
        typechecker_check_child(self, ast);
        self->cur_symtab = symtable_leavescope(self->cur_symtab);
        break;

    case AK_PARAMDECL:
        typechecker_check_child(self, ast);
        if (ast->type->kind
            == T_FUNCTION) //将所有参数的函数类型转换成函数指针类型
        {
            Type *type = type_new();
            type_init(type, T_POINTER);
            type->t.pointee_type = ast->type;
            type->flag = ast->type->flag;
            ast->type = type;
        }
    case AK_VARDECL: // TODO: 根据initlist确定补全数组长度
        NEWVAR(ast->val, ast->type, ast);
        typechecker_check_child(self, ast);
        break;
    case AK_RECORDDECL:
        type = type_new();
        if (CHECK_FLAG(ast->flag, AF_STRUCT))
            type_init(type, T_STRUCT);
        else
            type_init(type, T_UNION);
        type->name = ast->val;
        type->t.fields = symtable_new();
        symtable_init(type->t.fields);
        SymTable *_cur = self->cur_symtab;
        self->cur_symtab = type->t.fields;
        typechecker_check_brother(self, ast->recorddecl_field);
        self->cur_symtab = _cur;
        symtable_add_var_type(self->cur_symtab, type);
        break;
    case AK_FIELDDECL:
        if (ast->type->kind == T_FUNCTION)
            errora(ast->context, "field '%s' declared as a function", ast->val);
        NEWVAR(ast->val, ast->type, ast);
        tabitem->bit_field = ast->field_bit;
        break;
    case AK_ENUMDECL:
        type = type_new();
        type_init(type, T_ENUM);
        type->name = ast->val;
        e = type->t.constants;
        p = ast->enum_consts;
        while (p != NULL)
        {
            EnumAttribute *t = enumattr_new();
            enumattr_init(t, p->val, type, p->enumconst_val);
            NEWVAR(p->val, type, p);
            if (e == NULL)
                type->t.constants = e = t;
            else
            {
                e->next = t;
                t->pre = e;
                e = t;
            }
            p = p->sibling;
        }
        symtable_add_var_type(self->cur_symtab, type);
        break;
    case AK_TYPEDEFDECL:
        type = type_new();
        type_init(type, T_TYPEDEF);
        type->name = ast->val;
        type->t.typedef_type = ast->type;
        symtable_add_var_type(self->cur_symtab, type);
        break;
    case AK_NAME:
        tabitem = symtable_find_var(self->cur_symtab, ast->val);
        if (tabitem == NULL)
            errora(ast->context, "use of undeclared identifier '%s'", ast->val);
        ast->type = tabitem->type;
        break;
    case AK_CHAR: NEWTYPE(ast->type, T_CHAR); break;
    case AK_STRING:
        NEWTYPE(ast->type, T_ARRAY);
        NEWTYPE(ast->type->t.array.element_type, T_CHAR);
        NEWAST(ast->type->t.array.size, AK_INTEGER, ast->context);
        ast->type->t.array.size->val = from_uint64(strlen(ast->val));
        break;
    case AK_FLOAT: NEWTYPE(ast->type, T_DOUBLE); break;
    case AK_INTEGER: NEWTYPE(ast->type, T_LONGLONG); break;
    case AK_MEMBER:
        typechecker_check_child(self, ast);
        type = ast->member_target->type;
        if (type == NULL || type->kind != T_STRUCT && type->kind != T_UNION)
            errora(ast->context, "'%s' is not a struct or union",
                   type_tostr(type));
        tabitem = symtable_find_var(type->t.fields, ast->val);
        if (tabitem == NULL)
            errora(ast->context, "no member named '%s' in '%s'", ast->val,
                   type_tostr(type));
        ast->type = tabitem->type;
        break;
    case AK_EXPLICITCAST:
        typechecker_check_child(self, ast);
        ast->type = ast->cast_type->type;
        break;
    case AK_CALL:
        typechecker_check_child(self, ast);
        type = ast->call_func->type;
        if (type == NULL || type->kind != T_FUNCTION
            || (type->kind == T_POINTER
                && type->t.pointee_type->kind != T_FUNCTION))
            errora(
                ast->context,
                "called object type '%s' is not a function or function pointer",
                type_tostr(type));
        if (type->kind == T_FUNCTION)
            ast->type = type->t.function.return_type;
        else if (type->kind == T_POINTER)
            ast->type = type->t.pointee_type;
        break;
    case AK_SUBSCRIPT: // TODO: 隐式类型转换
        typechecker_check_child(self, ast);
        type = ast->subscript_target->type;
        if (type == NULL || type->kind != T_ARRAY && type->kind != T_POINTER)
            errora(ast->context, "subscripted value is not an array, pointer");
        if (type->kind == T_ARRAY)
            ast->type = type->t.array.element_type;
        else if (type->kind == T_POINTER)
            ast->type = type->t.pointee_type;
        break;
    case AK_BINARYOPERATOR:
    case AK_UNARYOPERATOR:
        typechecker_check_child(self, ast);
        ast->type = ast->child[0]->type; // TODO: 更准确地判断
        break;
    case AK_CONDITIONALOPERATOR:
        typechecker_check_child(self, ast);
        ast->type = ast->condop_true->type; // TODO: 更准确地判断
        break;
    case AK_TYPEORNAME:
        tabitem =
            symtable_find_var(self->cur_symtab, ast->typeorname_name->val);
        type = typechecker_complete_type(self, ast->typeorname_type->type);
        if (tabitem != NULL && type == NULL)                // name
            memcpy(ast, ast->typeorname_name, sizeof(AST)); //强行更改当前节点
        else if (tabitem == NULL && type != NULL)           // type
            memcpy(ast, ast->typeorname_type, sizeof(AST));
        else
            errora(
                ast->context,
                "Unable to determine whether the operand is an identifier or a typedef name.");
        typechecker_check(self, ast); //重新检查
        break;
    default: typechecker_check_child(self, ast);
    }
}