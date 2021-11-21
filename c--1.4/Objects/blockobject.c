#include "blockobject.h"

struct Object* BlockObject_New()
{
	struct BlockObject* block = (struct BlockObject*)malloc(sizeof(struct BlockObject));
	block->objattr = &BlockObjectAttribute;
	block->flags = ListObject_New();
	return (struct Object*)block;
}