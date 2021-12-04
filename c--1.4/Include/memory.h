#pragma once

#include "object.h"

#define FREELISTINIT(TYPE,OBJECT) \
		Memory_NewBlock(memory);\
		TYPE##_freelist = memory->tail;\
		TYPE##_freelist->obj = (struct OBJECT*)malloc(sizeof(struct OBJECT));\
		TYPE##_freelist->obj->block = TYPE##_freelist;

#define MALLOCOBJ(TYPE,OBJECT)	\
		struct Object* obj = TYPE##_freelist->obj;\
		TYPE##_freelist = TYPE##_freelist->back;\
		if (TYPE##_freelist == NULL)\
		{\
			Memory_NewBlock(mem);\
			TYPE##_freelist = mem->tail;\
			TYPE##_freelist->obj = (struct OBJECT*)malloc(sizeof(struct OBJECT));\
			TYPE##_freelist->obj->block = TYPE##_freelist;\
		}\
		return obj;

#define FREEOBJ(TYPE) \
		object->block->back = TYPE##_freelist;\
		TYPE##_freelist = object->block;

struct Block
{
	struct Block* next;	//供block使用
	struct Block* back;	//供freelist使用
	struct Object* obj;
};

struct Memory
{
	struct Block* head;
	struct Block* tail;
	int size;
};

struct Memory* memory;

struct Block* int_freelist;
struct Block* list_freelist;
struct Block* string_freelist;
struct Block* dict_freelist;
struct Block* frame_freelist;

struct Block* Block_New();
struct Memory* Memory_New();
void Memory_Init();
void Memory_NewBlock(struct Memory*);
struct Object* Memory_Malloc(struct Memory*, char*);
void Memory_Free(struct Memory*, struct Object*);