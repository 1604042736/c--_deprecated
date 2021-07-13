#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "symbol.h"
#include "error.h"

struct Analyzer
{
	struct SyntaxTree* syntaxtree;
	struct Symbol* symbol;
};

struct Analyzer* analyzer_init(struct SyntaxTree* syntaxtree,struct Symbol* symbol);
void analyzer_check(struct Analyzer* analyzer,struct SyntaxTree* t,char* filename);