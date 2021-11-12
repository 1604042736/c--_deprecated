#include "builtins.h"

Object* builtins_init()
{
	DictObject* builtinsdict = (DictObject*)DictObject_NewSimple();

	Object* name = StringObject_NewFromStringSimple("print");
	Object* print = CFunctionObject_New(name, builtins_print);
	builtinsdict->objattr->obj_dictadditem((Object*)builtinsdict, name, print);

	name = StringObject_NewFromStringSimple("memory");
	Object* memory= CFunctionObject_New(name, builtins_memory);
	builtinsdict->objattr->obj_dictadditem((Object*)builtinsdict, name, memory);

	name = StringObject_NewFromStringSimple("max");
	Object* max = CFunctionObject_New(name, builtins_max);
	builtinsdict->objattr->obj_dictadditem((Object*)builtinsdict, name, max);

	name = StringObject_NewFromStringSimple("min");
	Object* min = CFunctionObject_New(name, builtins_min);
	builtinsdict->objattr->obj_dictadditem((Object*)builtinsdict, name, min);

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

Object* builtins_memory(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	cout << arglist->list[0];
	NORMALRETURN;
}

Object* builtins_max(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	if (arg1->objattr->obj_gt(arg1, arg2))
	{
		return arg1;
	}
	return arg2;
}

Object* builtins_min(Object* args)
{
	ListObject* arglist = (ListObject*)args;
	Object* arg1 = arglist->list[0];
	Object* arg2 = arglist->list[1];
	if (arg1->objattr->obj_lt(arg1, arg2))
	{
		return arg1;
	}
	return arg2;
}