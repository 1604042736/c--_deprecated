#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "stringobject.h"
#include "dictobject.h"
#include "parser.h"

struct Preprocessor
{
	FILE* file;
	struct StringObject* result;
	char ch;
	struct DictObject* definedict;
	int defineflag;
};

struct Preprocessor* Preprocessor_New(FILE*);
void Preprocessor_Preprocess(struct Preprocessor*);
struct StringObject* Preprocessor_Dealprecode(struct Preprocessor*,struct StringObject*);