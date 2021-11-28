#pragma once

#include "dictobject.h"
#include "listobject.h"
#include "stringobject.h"
#include "cfunctionobject.h"
#include "exceptionobject.h"

struct DictObject* builtins;
struct DictObject* builtinnamespaces;

void Builtins_Init();

struct Object* Builtins_Print(struct Object*);