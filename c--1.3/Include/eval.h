#pragma once

#include "object.h"
#include "intobject.h"
#include "frameobject.h"
#include "opcode.h"
#include "cfunctionobject.h"
#include "functionobject.h"
#include "listobject.h"
#include "dictobject.h"

#define PUSHNAME(FRAME,ARG) ListObject_AddItem(FRAME->stack, ListObject_GetItem(FRAME->locals,ARG))
#define PUSHVAL(FRAME,VAL) ListObject_AddItem(FRAME->stack,VAL)
#define PUSHCONST(FRAME,ARG) ListObject_AddItem(FRAME->stack, ListObject_GetItem(codeobj->consts, ARG));
#define TOP(NAME,STACK)	Object* NAME=((ListObject*)STACK)->list.back();((ListObject*)STACK)->list.pop_back();
#define PUSH(FRAME,OBJ) ListObject_AddItem(FRAME->stack, OBJ)

Object* frame_eval(Object*);
Object* make_function(Object*, Object*,Object*,Object*);
Object* call_function(Object*,Object*, Object*);