#include "memory.h"
#include "intobject.h"
#include "listobject.h"
#include "stringobject.h"
#include "frameobject.h"

struct Block* Block_New()
{
	struct Block* block = (struct Block*)malloc(sizeof(struct Block));
	block->next = NULL;
	block->back = NULL;
	block->obj = NULL;
	return block;
}

struct Memory* Memory_New()
{
	struct Memory* mem = (struct Memory*)malloc(sizeof(struct Memory));
	mem->head = Block_New();
	mem->tail = mem->head;
	mem->size = 1;
	return mem;
}

void Memory_Init()
{
	memory = Memory_New();

	FREELISTINIT(int,IntObject)
	FREELISTINIT(list,ListObject)
	FREELISTINIT(string,StringObject)
	FREELISTINIT(dict,DictObject)
	FREELISTINIT(frame,FrameObject)
}

void Memory_NewBlock(struct Memory* mem)
{
	struct Block* block = Block_New();
	mem->tail->next = block;
	mem->tail = mem->tail->next;
	mem->size++;
}

struct Object* Memory_Malloc(struct Memory* mem, char* type)
{
	/*if (mem->size >= 1000)
	{
		exit(-1);
	}*/
	if (!strcmp(type, "int"))
	{
		MALLOCOBJ(int, IntObject)
		/*struct Object* obj = int_freelist->obj;
		int_freelist = int_freelist->back;
		printf("申请%p\n",obj);
		if (int_freelist == NULL)
		{
			Memory_NewBlock(mem);
			int_freelist = mem->tail;
			int_freelist->obj = (struct IntObject*)malloc(sizeof(struct IntObject));
			int_freelist->obj->block = int_freelist;
			printf("\t申请%p\n",int_freelist->obj);
		}
		return obj;*/
	}
	else if (!strcmp(type, "list"))
	{
		MALLOCOBJ(list, ListObject)
	}
	else if (!strcmp(type, "string"))
	{
		MALLOCOBJ(string,StringObject)
	}
	else if (!strcmp(type, "dict"))
	{
		MALLOCOBJ(dict, DictObject)
		/*struct Object* obj = dict_freelist->obj;
		dict_freelist = dict_freelist->back;
		if (dict_freelist == NULL)
		{
			Memory_NewBlock(mem);
			dict_freelist = mem->tail;
			dict_freelist->obj = (struct DictObject*)malloc(sizeof(struct DictObject));
			dict_freelist->obj->block = dict_freelist;
			printf("申请内存dict\n");
		}
		return obj;*/
	}
	else if (!strcmp(type, "frame"))
	{
		MALLOCOBJ(frame, FrameObject)
		/*struct Object* obj = frame_freelist->obj;
		frame_freelist = frame_freelist->back;
		if (frame_freelist == NULL)
		{
			Memory_NewBlock(mem);
			frame_freelist = mem->tail;
			frame_freelist->obj = (struct FrameObject*)malloc(sizeof(struct FrameObject));
			frame_freelist->obj->block = frame_freelist;
		}
		return obj;*/
	}
}

void Memory_Free(struct Memory* mem, struct Object* object)
{
	if (CHECK(object,"int"))
	{
		FREEOBJ(int)
		/*object->block->back = int_freelist;
		int_freelist = object->block;
		printf("\t\t释放%p\n",object);*/
	}
	//释放list时,要把每个item的refcount-1
	else if (CHECK(object, "list"))
	{
		struct ListObject* obj = (struct ListObject*)object;
		for (int i = 0; i < obj->size; i++)
		{
			SUBREFCOUNT(obj->item[i])
		}
		FREEOBJ(list)
		free(obj->item);
	}
	else if (CHECK(object, "string"))
	{
		FREEOBJ(string)
		free(((struct StringObject*)object)->string);
	}
	else if (CHECK(object, "dict"))
	{
		struct DictObject* obj = (struct DictObject*)object;
		for (int i = 0; i < obj->allocated; i++)
		{
			if (obj->item[i].key != NULL)
			{
				SUBREFCOUNT(obj->item[i].key)
				SUBREFCOUNT(obj->item[i].value)
			}
		}
		FREEOBJ(dict)
		/*object->block->back = dict_freelist;
		dict_freelist = object->block;*/
		free(obj->item);
	}
	else if (CHECK(object, "frame"))
	{
		struct FrameObject* obj=(struct FrameObject*)object;
		SUBREFCOUNT(obj->globals)
		SUBREFCOUNT(obj->locals)
		SUBREFCOUNT(obj->statck)
		FREEOBJ(frame)
		/*object->block->back = frame_freelist;
		frame_freelist = object->block;*/
	}
}