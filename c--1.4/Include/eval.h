#pragma once

#include "frameobject.h"
#include "intobject.h"
#include "functionobject.h"
#include "stringobject.h"
#include "cfunctionobject.h"
#include "blockobject.h"

struct Object* Eval(struct FrameObject*);
struct FunctionObject* make_function(struct FrameObject*,struct StringObject*, struct ByteCodeObject*);
struct Object* call(struct Object*, struct ListObject*);