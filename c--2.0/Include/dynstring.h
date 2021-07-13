#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996)

struct DynString
{
	char* str;
};

struct DynString* dynstring_init();
void dynstring_add_chr(struct DynString* self, char other);