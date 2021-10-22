#pragma once

#include "object.h"

struct CodeObject
{
	OBJECT_HEAD;
	Object* names;	//名字表
	Object* consts;	//常量表
	Object* code;	//代码
};

Object* CodeObject_New();
void CodeObject_Print(Object*);