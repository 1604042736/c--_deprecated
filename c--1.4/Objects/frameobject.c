#include "frameobject.h"

struct Object* FrameObject_New()
{
	struct FrameObject* frameobj = (struct FrameObject*)malloc(sizeof(struct FrameObject));
	if (frameobj == NULL)
	{
		printf("frame·ÖÅäÄÚ´æÊ§°Ü");
		exit(-1);
	}
	frameobj->objattr = &FrameObjectAttribute;
	frameobj->globals = DictObject_New();
	frameobj->locals = DictObject_New();
	frameobj->statck = ListObject_New();
	return (struct Object*)frameobj;
}

struct Object* FrameObject_NewWithByteCode(struct ByteCodeObject* bytecode)
{
	struct FrameObject* frameobj = FrameObject_New();
	frameobj->bytecode = bytecode;
	return (struct Object*)frameobj;
}

void FrameObject_Print(struct Object* self)
{
	struct FrameObject* selframe = (struct FrameObject*)self;
	printf("\n");
	ListObject_Print(selframe->statck);
	printf("\n");
	DictObject_Print(selframe->globals);
	printf("\n");
	DictObject_Print(selframe->locals);
	printf("\n");
}