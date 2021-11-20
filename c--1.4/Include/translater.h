#pragma once

#include "bytecodeobject.h"
#include "astobject.h"
#include "intobject.h"
#include "dictobject.h"

struct Translater
{
	struct ByteCodeObject* bytecode;
};

static struct DictObject* operationmap;
void Translater_Init();

struct Translater* Translater_New();
void Translater_Translate(struct Translater*,struct Object*);