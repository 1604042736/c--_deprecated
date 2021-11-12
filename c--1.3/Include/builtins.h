#pragma once

#include "object.h"
#include "dictobject.h"
#include "cfunctionobject.h"
#include "stringobject.h"
#include "listobject.h"
#include "intobject.h"

Object* builtins_init();
Object* builtins_print(Object*);
Object* builtins_memory(Object*);
Object* builtins_max(Object*);
Object* builtins_min(Object*);