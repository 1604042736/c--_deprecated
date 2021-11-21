#pragma once

#include "object.h"
#include "listobject.h"
#include "parser.h"

#define ASTOBJECT_HEAD	OBJECT_HEAD;int lineno,linepos

#define printSpace(SPACE) for (int i = 0; i < (SPACE); i++){printf("  ");}
#define checkASTObject(obj) (CHECK(obj,"Namespace"))

struct NameSpaceASTObject
{
	ASTOBJECT_HEAD;
	struct ListObject* body;
};

struct Object* NameSpaceASTObject_New();
struct Object* NameSpaceASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute NameSpaceASTObjectAttribute = {
	(char*)"Namespace",	//obj_name
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	NameSpaceASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct IfASTObject
{
	ASTOBJECT_HEAD;
	struct Object* exp;
	struct ListObject* body;
	struct ListObject* elses;
};

struct Object* IfASTObject_New();
struct Object* IfASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute IfASTObjectAttribute = {
	(char*)"If",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	IfASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct OperationASTObject
{
	ASTOBJECT_HEAD;
	struct Object* left;
	struct StringObject* op;
	struct Object* right;
};

struct Object* OperationASTObject_New();
struct Object* OperationASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute OperationASTObjectAttribute = {
	(char*)"Operation",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	OperationASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct NameASTObject
{
	ASTOBJECT_HEAD;
	struct StringObject* id;
	struct StringObject* mode;
};

struct Object* NameASTObject_New();
struct Object* NameASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute NameASTObjectAttribute = {
	(char*)"Name",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	NameASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct ConstantASTObject
{
	ASTOBJECT_HEAD;
	struct Object* value;
};

struct Object* ConstantASTObject_New();
struct Object* ConstantASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute ConstantASTObjectAttribute = {
	(char*)"Constant",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	ConstantASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct AssignASTObject
{
	ASTOBJECT_HEAD;
	struct ListObject* targets;
	struct Object* value;
};

struct Object* AssignASTObject_New();
struct Object* AssignASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute AssignASTObjectAttribute = {
	(char*)"Assign",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	AssignASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct SubscriptASTObject
{
	ASTOBJECT_HEAD;
	struct Object* value;
	struct Object* slice;
	struct StringObject* mode;
};

struct Object* SubscriptASTObject_New();
struct Object* SubscriptASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute SubscriptASTObjectAttribute = {
	(char*)"Subscript",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	SubscriptASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct CallASTObject
{
	ASTOBJECT_HEAD;
	struct Object* func;
	struct ListObject* args;
	struct StringObject* mode;
};

struct Object* CallASTObject_New();
struct Object* CallASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute CallASTObjectAttribute = {
	(char*)"Call",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	CallASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct ReturnASTObject
{
	ASTOBJECT_HEAD;
	struct Object* value;
};

struct Object* ReturnASTObject_New();
struct Object* ReturnASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute ReturnASTObjectAttribute = {
	(char*)"Return",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	ReturnASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct FunctionDefASTObject
{
	ASTOBJECT_HEAD;
	struct StringObject* name;
	struct ListObject* args;
	struct ListObject* body;
};

struct Object* FunctionDefASTObject_New();
struct Object* FunctionDefASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute FunctionDefASTObjectAttribute = {
	(char*)"FunctionDef",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	FunctionDefASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct WhileASTObject
{
	ASTOBJECT_HEAD;
	struct Object* exp;
	struct ListObject* body;
};

struct Object* WhileASTObject_New();
struct Object* WhileASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute WhileASTObjectAttribute = {
	(char*)"While",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	WhileASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct ListASTObject
{
	ASTOBJECT_HEAD;
	struct ListObject* list;
};

struct Object* ListASTObject_New();
struct Object* ListASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute ListASTObjectAttribute = {
	(char*)"List",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	ListASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct AttributeASTObject
{
	ASTOBJECT_HEAD;
	struct Object* value;
	struct StringObject* attr;
	struct StringObject* mode;
};

struct Object* AttributeASTObject_New();
struct Object* AttributeASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute AttributeASTObjectAttribute = {
	(char*)"Attribute",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	AttributeASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct BreakASTObject
{
	ASTOBJECT_HEAD;
};

struct Object* BreakASTObject_New();
struct Object* BreakASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute BreakASTObjectAttribute = {
	(char*)"Break",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	BreakASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct ContinueASTObject
{
	ASTOBJECT_HEAD;
};

struct Object* ContinueASTObject_New();
struct Object* ContinueASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute ContinueASTObjectAttribute = {
	(char*)"Continue",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	ContinueASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

struct ImportASTObject
{
	ASTOBJECT_HEAD;
	struct Object* name;
};

struct Object* ImportASTObject_New();
struct Object* ImportASTObject_NewWithParser(struct Parser*);

static struct ObjectAttribute ImportASTObjectAttribute = {
	(char*)"Import",
	NULL,	//obj_add
	NULL,	//obj_and
	NULL,	//obj_bool
	NULL,	//obj_div
	NULL,	//obj_eq
	NULL,	//obj_getattr
	NULL,	//obj_getitem
	NULL,	//obj_geq
	NULL,	//obj_gt
	NULL,	//obj_insertitem
	NULL,	//obj_leq
	NULL,	//obj_lt
	NULL,	//obj_mod
	NULL,	//obj_mul
	NULL,	//obj_neq
	ImportASTObject_New,	//obj_new
	NULL,	//obj_or
	NULL,	//obj_print
	NULL,	//obj_setattr
	NULL,	//obj_setitem
	NULL,	//obj_sub
};

void printastobject(struct Object*,int);