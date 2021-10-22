#pragma once

#include "object.h"
#include "intobject.h"
#include "frameobject.h"
#include "opcode.h"
#include "cfunctionobject.h"
#include "functionobject.h"

#define PUSHNAME(FRAME,ARG) FRAME->stack->objattr->obj_additem(FRAME->stack, FRAME->locals->objattr->obj_getitem(FRAME->locals,ARG))
#define PUSHVAL(FRAME,VAL) FRAME->stack->objattr->obj_additem(FRAME->stack,VAL)
#define PUSHCONST(FRAME,ARG) FRAME->stack->objattr->obj_additem(FRAME->stack, consts->objattr->obj_getitem(codeobj->consts, ARG));
#define TOP(NAME,STACK)	Object* NAME=((ListObject*)STACK)->list.back();((ListObject*)STACK)->list.pop_back();
#define PUSH(FRAME,OBJ) FRAME->stack->objattr->obj_additem(FRAME->stack, OBJ)

Object* frame_eval(Object*);
Object* make_function(Object*, Object*,Object*,Object*);
Object* call_function(Object*,Object*, Object*);