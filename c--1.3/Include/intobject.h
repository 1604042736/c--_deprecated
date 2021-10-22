#pragma once

#include "object.h"

struct IntObject
{
	OBJECT_HEAD;
	int val;
};

Object* IntObject_NewFromInt(int);
void IntObject_Print(Object*);
bool IntObject_Eq(Object*, Object*);
Object* IntObject_Add(Object*, Object*);
Object* IntObject_Sub(Object*, Object*);
Object* IntObject_Mul(Object*, Object*);
Object* IntObject_Div(Object*, Object*);
Object* IntObject_Mod(Object*, Object*);
bool IntObject_Bool(Object*);
bool IntObject_Or(Object*, Object*);
bool IntObject_Gt(Object*, Object*);
bool IntObject_Geq(Object*, Object*);
bool IntObject_Lt(Object*, Object*);
bool IntObject_Leq(Object*, Object*);
bool IntObject_Neq(Object*, Object*);
bool IntObject_And(Object*, Object*);