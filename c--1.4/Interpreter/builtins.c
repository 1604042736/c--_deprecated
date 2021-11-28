#include "builtins.h"
#include "namespace_system.h"

void Builtins_Init()
{
#define ADD(NAME,CFUNC) DictObject_SetItem(builtins,StringObject_NewWithString(NAME),CFunctionObject_NewWithCFunc(NAME,CFUNC))
	
	builtins = DictObject_New();
	ADD("print", Builtins_Print);

#undef ADD

#define ADD(NAME,NAMESPACE) DictObject_SetItem(builtinnamespaces,StringObject_NewWithString(NAME),NAMESPACE)

	builtinnamespaces = DictObject_New();
	ADD("system", namespace_system);

#undef ADD
}

struct Object* Builtins_Print(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	for (int i = 0; i < args->size; i++)
	{
		struct Object* obj = args->item[i];
		if (CHECK(obj, "string"))
		{
			printf("%s", ((struct StringObject*)obj)->string);
		}
		else if(obj->objattr->obj_print!=NULL)
		{
			obj->objattr->obj_print(obj);
		}
		else
		{
			struct StringObject* str = Object_ToString(obj);
			if (str != NULL)
			{
				printf("%s", str->string);
			}
		}
	}
	NORMALRETURN
}