#pragma once

#include "object.h"
#include "codeobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "builtins.h"

struct FrameObject
{
	OBJECT_HEAD;
	Object* codeobj;	//����
	Object* builtins;	//�ڽ�
	Object* globals;	//ȫ��������
	Object* locals;		//�ֲ�������
	Object* stack;	//ջ
};

Object* FrameObject_NewFrameCodeObject(Object*);
Object* FrameObject_Copy(Object*);