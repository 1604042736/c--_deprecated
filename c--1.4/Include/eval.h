#pragma once

#include "frameobject.h"
#include "intobject.h"
#include "functionobject.h"
#include "stringobject.h"
#include "cfunctionobject.h"
#include "blockobject.h"
#include "namespaceobject.h"
#include "classobject.h"
#include "exceptionobject.h"
#include "builtins.h"

struct Object* Eval(struct FrameObject*);
struct FunctionObject* make_function(struct FrameObject*,struct StringObject*, struct ByteCodeObject*);
struct Object* call(struct Object*, struct ListObject*);
struct NamespaceObject* load_namespace(struct StringObject*);
struct NamespaceObject* compiler(char*,char*,int);