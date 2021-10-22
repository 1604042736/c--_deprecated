#pragma once

#include "object.h"
#include "dictobject.h"
#include "cfunctionobject.h"
#include "stringobject.h"
#include "listobject.h"
#include "intobject.h"

#define NORMALRETURN return IntObject_NewFromInt(0);

Object* builtins_init();
Object* builtins_print(Object*);