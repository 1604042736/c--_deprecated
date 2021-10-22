#pragma once

#include<iostream>
#include "object.h"

struct StringObject
{
	OBJECT_HEAD;
	string str;
};

Object* StringObject_NewFromString(string);
void StringObject_Print(Object*);
bool StringObject_Eq(Object*,Object*);
Object* StringObject_Add(Object*, Object*);
bool StringObject_Bool(Object*);