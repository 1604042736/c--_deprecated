#include "bytecodeobject.h"
#include "eval.h"
#include "frameobject.h"

struct Object* ByteCodeObject_New()
{
	struct ByteCodeObject* bytecodeobj = (struct ByteCodeObject*)malloc(sizeof(struct ByteCodeObject));
	bytecodeobj->objattr = &ByteCodeObjectAttribute;
	bytecodeobj->code = ListObject_New();
	bytecodeobj->consts = ListObject_New();
	bytecodeobj->name = "";
	bytecodeobj->refcount = DEFAULTREFCOUNT;
	return (struct Object*)bytecodeobj;
}

void ByteCodeObject_Print(struct Object* self)
{
	struct ByteCodeObject* selfbytecode = (struct ByteCodeObject*)self;
	printf("<ByteCode %s at %p>", selfbytecode->name, selfbytecode);
}

void ByteCodeObject_Print2(struct Object* self)
{
	struct ByteCodeObject* selfbytecode = (struct ByteCodeObject*)self;
	struct ListObject* bytecodelist = ListObject_New();
	ListObject_Print(selfbytecode->consts);
	for (int i = 0; i < selfbytecode->consts->size; i++)
	{
		if (CHECK(selfbytecode->consts->item[i], "bytecode"))
		{
			ListObject_InsertItem(bytecodelist, bytecodelist->size, selfbytecode->consts->item[i]);
		}
	}
	printf("\nOp\tOparg\tLineno\tLinepos\n");
	for (int i = 0; i < selfbytecode->code->size; i++)
	{
		OpCodeObject_Print(selfbytecode->code->item[i]);
		printf("\n");
	}
	for (int i = 0; i < bytecodelist->size; i++)
	{
		ByteCodeObject_Print(bytecodelist->item[i]);
		printf("\n");
		ByteCodeObject_Print2(bytecodelist->item[i]);
	}
}

struct Object* ByteCodeObject_Call(struct Object* self, struct Object* args)
{
	struct FrameObject* ft = FrameObject_NewWithByteCode(self);
	return Eval(ft);
}