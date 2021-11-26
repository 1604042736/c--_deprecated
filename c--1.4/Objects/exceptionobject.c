#include "exceptionobject.h"

struct Object* ExceptionObject_New()
{
	struct ExceptionObject* exceptobj = (struct ExceptionObject*)malloc(sizeof(struct ExceptionObject));
	exceptobj->objattr = &ExceptionObjectAttribute;
	exceptobj->message = StringObject_New();
	return (struct Object*)exceptobj;
}

struct Object* ExceptionObject_NewWithMessage(struct StringObject* message, struct StringObject* filename,int lineno,int linepos)
{
	struct ExceptionObject* exceptobj = (struct ExceptionObject*)ExceptionObject_New();
	exceptobj->message = message;
	exceptobj->lineno = lineno;
	exceptobj->linepos = linepos;
	exceptobj->filename = filename;
	return (struct Object*)exceptobj;
}

void ExceptionObject_Print(struct Object* self)
{
	
}