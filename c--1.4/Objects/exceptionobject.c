#include "exceptionobject.h"

struct Object* ExceptionObject_New()
{
	struct ExceptionObject* exceptobj = (struct ExceptionObject*)malloc(sizeof(struct ExceptionObject));
	exceptobj->objattr = &ExceptionObjectAttribute;
	exceptobj->message = StringObject_New();
	return (struct Object*)exceptobj;
}

struct Object* ExceptionObject_NewWithMessage(char* message,int lineno,int linepos)
{
	struct ExceptionObject* exceptobj = (struct ExceptionObject*)ExceptionObject_New();
	exceptobj->message = StringObject_NewWithString(message);
	exceptobj->lineno = lineno;
	exceptobj->linepos = linepos;
	return (struct Object*)exceptobj;
}

void ExceptionObject_Print(struct Object* self)
{
	
}