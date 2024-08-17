#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "declanalyzer.h"
#include "error.h"
#include "stringutil.h"
#include "warning.h"

DeclAnalyzer *declanalyzer_new()
{
    return (DeclAnalyzer *)malloc(sizeof(DeclAnalyzer));
}

void declanalyzer_init(DeclAnalyzer *self)
{
    self->flag = 0;
    self->type = NULL;
    self->name = NULL;
    memset(self->param_names, 0, sizeof(self->param_names));
    memset(self->param_context, 0, sizeof(self->param_context));
    self->param_num = 0;
}

void declanalyzer_analyze_brother(DeclAnalyzer *self, AST *ast)
{
    AST *p = ast;
    while (p != NULL)
    {
        declanalyzer_analyze(self, p);
        p = p->sibling;
    }
}

void declanalyzer_analyze_child(DeclAnalyzer *self, AST *ast)
{
    for (int i = 0; i < MAX_AST_CHILD_NUM; i++)
    {
        declanalyzer_analyze(self, ast->child[i]);
        if (ast->child[i] != NULL)
            declanalyzer_analyze_brother(self, ast->child[i]->sibling);
    }
}

TypeKind get_typekind(char *s)
{
    if (!strcmp(s, "char")) return T_CHAR;
    if (!strcmp(s, "short")) return T_SHORT;
    if (!strcmp(s, "int")) return T_INT;
    if (!strcmp(s, "long")) return T_LONG;
    if (!strcmp(s, "void")) return T_VOID;
    if (!strcmp(s, "float")) return T_FLOAT;
    if (!strcmp(s, "double")) return T_DOUBLE;
    return T_TYPEDEF;
}

#define CHANGE_FLAG(f, op)                                                     \
    do                                                                         \
    {                                                                          \
        if (self->type != NULL) op(self->type->flag, f);                       \
        op(self->flag, f);                                                     \
    } while (0)

#define RESTORE                                                                \
    do                                                                         \
    {                                                                          \
        self->type = _type;                                                    \
        self->flag = _flag;                                                    \
        self->name = _name;                                                    \
    } while (0)

#define SAVE                                                                   \
    do                                                                         \
    {                                                                          \
        _type = self->type;                                                    \
        _name = self->name;                                                    \
        _flag = self->flag;                                                    \
    } while (0)

#define CLEAR                                                                  \
    do                                                                         \
    {                                                                          \
        self->type = NULL;                                                     \
        self->flag = 0;                                                        \
        self->name = NULL;                                                     \
    } while (0)

/*
根据ParamDeclaration获取参数列表
*/
AST *declanalyzer_get_param(DeclAnalyzer *self, AST *h, Type *f, int start,
                            int end)
{
    AST *t = h;
    ParamAttribute *p = f->t.function.param_types;
    int i = start;
    while (p != NULL && i < end)
    {
        AST *q = ast_new();
        ast_init(q, AK_PARAMDECL, self->param_context[i]);
        q->val = self->param_names[i++];
        q->type = p->type;
        if (t == NULL)
            t = h = q;
        else
        {
            t->sibling = q;
            t = t->sibling;
            t->head_sibling = h;
        }
        p = p->next;
    }
    return h;
}

void declanalyzer_analyze(DeclAnalyzer *self, AST *ast)
{
    if (ast == NULL) return;
    AST *p, *t;
    char buf[100];
    Type *_type, *type_;
    char *_name;
    int _flag, start, end;
    switch (ast->kind)
    {
    case AK_STORAGECLASSSPECIFIER:
        if (!strcmp(ast->val, "typedef"))
            CHANGE_FLAG(TF_TYPEDEF, ADD_FLAG);
        else if (!strcmp(ast->val, "extern"))
            CHANGE_FLAG(TF_EXTERN, ADD_FLAG);
        else if (!strcmp(ast->val, "static"))
            CHANGE_FLAG(TF_STATIC, ADD_FLAG);
        else if (!strcmp(ast->val, "auto"))
            CHANGE_FLAG(TF_AUTO, ADD_FLAG);
        else if (!strcmp(ast->val, "register"))
            CHANGE_FLAG(TF_REGISTER, ADD_FLAG);
        break;
    case AK_TYPESPECIFER:
        if (!strcmp(ast->val, "unsigned"))
            CHANGE_FLAG(TF_UNSIGNED, ADD_FLAG);
        else if (!strcmp(ast->val, "signed"))
            CHANGE_FLAG(TF_UNSIGNED, REMOVE_FLAG);
        else
        {
            if (self->type == NULL)
            {
                self->type = type_new();
                type_init(self->type, get_typekind(ast->val));
            }
            else if (self->type != NULL) //类型组合
            {
                TypeKind kind = get_typekind(ast->val);
                if (self->type->kind == T_INT && kind == T_LONG
                    || self->type->kind == T_LONG && kind == T_INT) // long int
                    self->type->kind = T_LONG;
                else if (self->type->kind == T_LONG
                         && kind == T_LONG) // long long
                    self->type->kind = T_LONGLONG;
                else
                {
                    errora(ast->context,
                           "cannot combine with '%s'(%d) and '%s'(%d)",
                           type_names[self->type->kind], self->type->kind,
                           type_names[kind], kind);
                }
            }
            if (self->type->kind == T_TYPEDEF) self->type->name = ast->val;
            self->type->flag = self->flag;
            //这两个flag只影响当前类型
            REMOVE_FLAG(self->flag, TF_CONST);
            REMOVE_FLAG(self->flag, TF_VOLATILE);
        }
        break;
    case AK_TYPEQUALIFIER:
        if (!strcmp(ast->val, "const"))
            CHANGE_FLAG(TF_CONST, ADD_FLAG);
        else if (!strcmp(ast->val, "volatile"))
            CHANGE_FLAG(TF_VOLATILE, ADD_FLAG);
        break;
    case AK_DECLNAME:
        self->name = ast->val;
        declanalyzer_analyze_child(self, ast);
        break;
    case AK_POINTERDECLARATOR: {
        Type *type = type_new();
        type_init(type, T_POINTER);
        type->flag = self->flag;
        type->t.pointee_type = self->type;
        self->type = type;
        declanalyzer_analyze_brother(self, ast->pdeclor_qualifier);

        REMOVE_FLAG(self->flag, TF_CONST);
        REMOVE_FLAG(self->flag, TF_VOLATILE);

        declanalyzer_analyze_brother(self, ast->pdeclor_sub);
        break;
    }
    case AK_ARRAYDECLARATOR: {
        Type *type = type_new();
        type_init(type, T_ARRAY);
        type->flag = self->flag;
        type->t.array.element_type = self->type;
        type->t.array.size = ast->arraydeclor_len;
        self->type = type;
        declanalyzer_analyze_brother(self, ast->arraydeclor_sub);
        break;
    }
    case AK_FUNCTIONDECLARATOR: {
        SAVE;
        Type *type = type_new();
        type_init(type, T_FUNCTION);
        type->flag = self->flag;
        type->t.function.return_type = self->type;

        CLEAR;

        ParamAttribute *h = NULL, *t = NULL;
        p = ast->funcdeclor_param;
        while (p != NULL)
        {
            if (p->kind == AK_VARPARAM)
            {
                type->t.function.has_varparm = 1;
                p = p->sibling;
                continue;
            }
            declanalyzer_analyze(self, p);
            ParamAttribute *q = paramattr_new();
            paramattr_init(q, self->type, type);
            self->param_names[self->param_num] = self->name;
            self->param_context[self->param_num] = p->context;
            self->param_num++;
            if (h == NULL)
                h = t = q;
            else
            {
                t->next = q;
                q->pre = t;
                t = t->next;
            }
            p = p->sibling;
            CLEAR;
        }
        type->t.function.param_types = h;

        RESTORE;
        self->type = type;

        declanalyzer_analyze(self, ast->funcdeclor_sub);
        break;
    }
    case AK_FUNCTIONDEF:
        SAVE;
        CLEAR;
        declanalyzer_analyze_brother(self, ast->funcdef_spec);
        start = self->param_num;
        declanalyzer_analyze_brother(self, ast->funcdef_declor);
        ast->type = self->type;
        ast->val = self->name;
        end = self->param_num;
        ast->funcdef_param = declanalyzer_get_param(self, ast->funcdef_param,
                                                    ast->type, start, end);
        self->param_num = start;
        RESTORE;
        declanalyzer_analyze(self, ast->funcdef_body);
        break;
    case AK_ENUMDECL:
        self->type = type_new();
        type_init(self->type, T_ENUM);
        if (ast->val[0] == '\0')
        {
            ast->val = empty_string();
            sprintf(buf, "<unnamed at (%d:%d)>", ast->context.s_row,
                    ast->context.s_col);
            ast->val = add_string(ast->val, buf);
        }
        self->type->name = ast->val;
        break;
    case AK_RECORDSPECIFIER:
        SAVE;
        CLEAR;
        if (ast->recordspec_list != NULL)
        {
            p = ast_new();
            ast_init(p, AK_RECORDDECL, ast->context);
            p->flag = ast->flag;
            ast->recordspec_decl = p;
            p = p->recorddecl_field;
            t = ast->recordspec_list; // t->kind==AK_PARAMDECLARATION
            while (t != NULL)
            {
                AST *_t = t;
                declanalyzer_analyze_brother(self, t->decl_spec);
                type_ = self->type;
                t = t->decl_declor;
                while (t != NULL)
                {
                    declanalyzer_analyze(self, t);

                    AST *q = ast_new();
                    ast_init(q, AK_FIELDDECL, t->context);
                    q->val = self->name;
                    q->type = self->type;
                    q->field_bit = t->recorddeclor_bit;

                    if (ast->recordspec_decl->recorddecl_field == NULL)
                        ast->recordspec_decl->recorddecl_field = p = q;
                    else
                    {
                        p->sibling = q;
                        p = p->sibling;
                        p->head_sibling =
                            ast->recordspec_decl->recorddecl_field;
                    }

                    self->type = type_;
                    self->name = "";
                    t = t->sibling;
                }
                CLEAR;
                t = _t->sibling;
            }
        }
        RESTORE;

        self->type = type_new();
        if (CHECK_FLAG(ast->flag, AF_STRUCT))
            type_init(self->type, T_STRUCT);
        else
            type_init(self->type, T_UNION);
        self->type->flag = self->flag;
        if (ast->val[0] == '\0')
        {
            self->type->name = empty_string();
            sprintf(buf, "<unnamed at (%d:%d)>", ast->context.s_row,
                    ast->context.s_col);
            self->type->name = add_string(self->type->name, buf);
        }
        else
            self->type->name = ast->val;
        if (ast->recordspec_decl != NULL) // ast->recordspec_list != NULL
            ast->recordspec_decl->val = self->type->name;
        break;
    case AK_DECLSTMT:
        SAVE;
        CLEAR;
        declanalyzer_analyze_brother(self, ast->decl_spec);
        if (ast->decl_declor == NULL)
        {
            warninga(ast->context, "declaration does not declare anything");
            RESTORE;
            break;
        }
        assert(ast->decl_declor->kind == AK_INITDECLARATOR);

        type_ = self->type; //保存类型说明
        p = ast->decl_declor;
        t = ast->decl_decls;
        while (p != NULL)
        {
            start = self->param_num;
            declanalyzer_analyze(self, p->initdecl_decl);
            //添加新的节点
            AST *new_ast = ast_new();
            if (CHECK_FLAG(self->flag, TF_TYPEDEF))
            {
                if (p->initdecl_init != NULL)
                {
                    errora(
                        ast->context,
                        "illegal initializer (only variables can be initialized)");
                    RESTORE;
                    break;
                }
                ast_init(new_ast, AK_TYPEDEFDECL, p->context);
            }
            else if (self->type->kind == T_FUNCTION)
            {
                ast_init(new_ast, AK_FUNCTIONDEF, p->context);
                end = self->param_num;
                new_ast->funcdef_param = declanalyzer_get_param(
                    self, new_ast->funcdef_param, self->type, start, end);
                self->param_num = start;
            }
            else
                ast_init(new_ast, AK_VARDECL, p->context);
            new_ast->val = self->name;
            new_ast->type = self->type;
            declanalyzer_analyze(self, p->initdecl_init);
            if (self->type->kind != T_FUNCTION)
                new_ast->vardecl_init = p->initdecl_init;
            if (t == NULL)
                ast->decl_decls = t = new_ast;
            else
            {
                t->sibling = new_ast;
                t = t->sibling;
                t->head_sibling = ast->decl_decls;
            }
            p = p->sibling;
            CLEAR;
            self->type = type_;
        }
        RESTORE;
        break;
    case AK_TYPENAME:
        SAVE;
        CLEAR;
        declanalyzer_analyze(self, ast->typename_specqual);
        declanalyzer_analyze(self, ast->typename_declor);
        ast->type = self->type;
        RESTORE;
        break;
    default: declanalyzer_analyze_child(self, ast);
    }
}