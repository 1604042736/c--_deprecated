#pragma once

#include "object.h"

struct CodeObject
{
	OBJECT_HEAD;
	Object* names;	//���ֱ�
	Object* consts;	//������
	Object* code;	//����
};

Object* CodeObject_New();
void CodeObject_Print(Object*);