#include "builtins.h"

Object* builtins_init()
{
	DictObject* builtinsdict = (DictObject*)DictObject_New();

	Object* print = CFunctionObject_New(StringObject_NewFromString("print"), builtins_print);
	builtinsdict->objattr->obj_dictadditem((Object*)builtinsdict, StringObject_NewFromString("print"), print);

	return (Object*)builtinsdict;
}

Object* builtins_print(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	for (int i = arglist->list.size()-1; i >=0; i--)
	{
		print(arglist->list[i]);
	}
	NORMALRETURN;
}