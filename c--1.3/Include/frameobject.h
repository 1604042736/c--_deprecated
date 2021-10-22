#pragma once

#include "object.h"
#include "codeobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "builtins.h"

struct FrameObject
{
	OBJECT_HEAD;
	Object* codeobj;	//代码
	Object* builtins;	//内建
	Object* globals;	//全局作用域
	Object* locals;		//局部作用域
	Object* stack;	//栈
};

Object* FrameObject_NewFrameCodeObject(Object*);
Object* FrameObject_Copy(Object*);