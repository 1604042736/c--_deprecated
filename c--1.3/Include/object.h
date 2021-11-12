#pragma once

#include <string>
#include <iostream>
#include <map>
using namespace std;

#define OBJECT_HEAD ObjectAttr* objattr
#define NORMALRETURN return IntObject_NewFromInt(0);
#define ADDCFUNCTODICT(NAME,CFUNC,OBJATTR,SELF) name = StringObject_NewFromStringSimple(NAME);\
								cfuncobj = (CFunctionObject*)CFunctionObject_New(name, CFUNC);\
								cfuncobj->self = (Object*)SELF;\
								DictObject_DictAddItem(OBJATTR->dict, name, (Object*)cfuncobj);

struct Object;
typedef Object* (*CFunction)(Object*);
typedef void (*PrintFunc)(Object*);
typedef void (*AddItem)(Object*,Object*);
typedef int (*FindItem)(Object*, Object*);
typedef Object* (*GetItem)(Object*, Object*);
typedef bool (*Eq)(Object*, Object*);
typedef void (*DictAddItem)(Object*, Object*, Object*);
typedef Object* (*Add)(Object*, Object*);
typedef Object* (*SUB)(Object*, Object*);
typedef Object* (*MUL)(Object*, Object*);
typedef Object* (*DIV)(Object*, Object*);
typedef Object* (*MOD)(Object*, Object*);
typedef void (*InsertItem)(Object*, Object*, Object*);
typedef bool (*Bool)(Object*);
typedef Object* (*Copy)(Object*);
typedef bool (*Or)(Object*, Object*);
typedef void (*SetItem)(Object*, Object*,Object*);
typedef bool (*Gt)(Object*, Object*);
typedef bool (*Geq)(Object*, Object*);
typedef bool (*Lt)(Object*, Object*);
typedef bool (*Leq)(Object*, Object*);
typedef bool (*Neq)(Object*, Object*);
typedef bool (*And)(Object*, Object*);
/*Object��һЩ����*/
struct ObjectAttr
{
	string obj_name;	//object������
	PrintFunc obj_print;	//���object
	AddItem obj_additem;	//���item
	FindItem obj_finditem;	//����item
	GetItem obj_getitem;	//��ȡitem
	DictAddItem obj_dictadditem;	//���dict��item
	InsertItem obj_insertitem;	//��ĳһλ�����item
	SetItem obj_setitem;	//����item
	Add obj_add;	//�ӷ�����
	SUB obj_sub;	//��������
	MUL obj_mul;	//�˷�����
	DIV obj_div;	//��������
	MOD obj_mod;	//ȡģ����
	Gt obj_gt;	//>����
	Geq obj_geq;	//>=����
	Lt obj_lt;
	Leq obj_leq;
	Eq obj_eq;	//����object�Ƿ����
	Neq obj_neq;
	And obj_and;
	Or obj_or;	//������
	Bool obj_bool;	//true or false
	Copy obj_copy;	//����

	Object* dict;
};

struct Object
{
	OBJECT_HEAD;
};

/*ObjectAttr* ObjectAttr_New(string);
Object* Object_New();
void Object_Print(Object*);
void Object_AddItem(Object*, Object*);
int Object_FindItem(Object*, Object*);
Object* Object_GetItem(Object*, Object*);
bool Object_Eq(Object*, Object*);
void Object_DictAddItem(Object*, Object*, Object*);
Object* Object_Add(Object*, Object*);
Object* Object_Sub(Object*, Object*);
Object* Object_Mul(Object*, Object*);
Object* Object_Div(Object*, Object*);
Object* Object_Mod(Object*, Object*);
void Object_InsertItem(Object*, Object*, Object*);
bool Object_Bool(Object*);
Object* Object_Copy(Object*);
bool Object_Or(Object*,Object*);
void Object_SetItem(Object*, Object*,Object*);
bool Object_Gt(Object*, Object*);
bool Object_Geq(Object*, Object*);
bool Object_Lt(Object*, Object*);
bool Object_Leq(Object*, Object*);
bool Object_Neq(Object*, Object*);
bool Object_And(Object*, Object*);

Object* ObjectDict_AddItem(Object*);
Object* ObjectDict_FindItem(Object*);
Object* ObjectDict_GetItem(Object*);
Object* ObjectDict_Eq(Object*);
Object* ObjectDict_DictAddItem(Object*);
Object* ObjectDict_Add(Object*);
Object* ObjectDict_Sub(Object*);
Object* ObjectDict_Mul(Object*);
Object* ObjectDict_Div(Object*);
Object* ObjectDict_Mod(Object*);
Object* ObjectDict_InsertItem(Object*);
Object* ObjectDict_Bool(Object*);
Object* ObjectDict_Copy(Object*);
Object* ObjectDict_Or(Object*);
Object* ObjectDict_SetItem(Object*);
Object* ObjectDict_Gt(Object*);
Object* ObjectDict_Geq(Object*);
Object* ObjectDict_Lt(Object*);
Object* ObjectDict_Leq(Object*);
Object* ObjectDict_Neq(Object*);
Object* ObjectDict_And(Object*);*/

void print(Object*);