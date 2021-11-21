#include "eval.h"

struct Object* Eval(struct FrameObject* frameobj)
{
#define BYTECODE	(frameobj->bytecode)
#define CODE		(BYTECODE->code)
#define STACK		(frameobj->statck)
	int i = 0,op,oparg,index;
	int size = frameobj->bytecode->code->size;
	struct Object* temp=Object_New();
	struct ListObject* blocks = ListObject_New();
	while (i < size)
	{
		op = ((struct OpCodeObject*)frameobj->bytecode->code->item[i])->op;
		oparg= ((struct OpCodeObject*)frameobj->bytecode->code->item[i])->oparg;
		switch (op)
		{
		case OP_LOAD_CONST:
			ListObject_InsertItem(STACK, STACK->size, BYTECODE->consts->item[oparg]);
			break;
		case OP_STORE_NAME:
			DictObject_SetItem(frameobj->locals, BYTECODE->consts->item[oparg], STACK->item[STACK->size - 1]);
			ListObject_ListDelItem(STACK, STACK->size - 1);
			break;
		case OP_LOAD_NAME:
		{
			struct Object* name = BYTECODE->consts->item[oparg];
			struct Object* value=DictObject_GetItem(frameobj->locals, name);
			if (value == NULL)
			{
				value=DictObject_GetItem(frameobj->globals, name);
				if (value == NULL)
				{
					printf("ÕÒ²»µ½");
					name->objattr->obj_print(name);
					exit(-1);
				}
			}
			ListObject_InsertItem(STACK, STACK->size, value);
			break;
		}
		case OP_ADD:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_add(left,right));
			break;
		}
		case OP_SUB:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_sub(left, right));
			break;
		}
		case OP_MUL:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_mul(left, right));
			break;
		}
		case OP_DIV:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_div(left, right));
			break;
		}
		case OP_MOD:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_mod(left, right));
			break;
		}
		case OP_EQ:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size,IntObject_NewWithValue( left->objattr->obj_eq(left, right)));
			break;
		}
		case OP_NEQ:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_neq(left, right));
			break;
		}
		case OP_GT:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_gt(left, right));
			break;
		}
		case OP_GEQ:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_geq(left, right));
			break;
		}
		case OP_LT:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_lt(left, right));
			break;
		}
		case OP_LEQ:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_leq(left, right));
			break;
		}
		case OP_AND:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_and(left, right));
			break;
		}
		case OP_OR:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, left->objattr->obj_or(left, right));
			break;
		}
		case OP_IFJMP:
		{
			struct Object* val = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			if (val->objattr->obj_bool && !val->objattr->obj_bool(val))
			{
				i += oparg;
			}
			break;
		}
		case OP_JMP:
		{
			i += oparg;
			break;
		}
		case OP_RETURN:
		{
			struct Object* val = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			return val;
			break;
		}
		case OP_MAKE_FUNCTION:
		{
			struct ByteCodeObject* bytecode= STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct StringObject* name = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct FunctionObject* func = make_function(frameobj,name, bytecode);
			ListObject_InsertItem(STACK, STACK->size, func);
			break;
		}
		case OP_CALL:
		{
			struct Object* func = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct ListObject* args = ListObject_New();
			for (int i = 0; i < oparg; i++)
			{
				ListObject_InsertItem(args, args->size, STACK->item[STACK->size - 1]);
				ListObject_ListDelItem(STACK, STACK->size - 1);
			}
			ListObject_InsertItem(STACK, STACK->size, call(func,args));
			break;
		}
		case OP_STORE_ARG:
		{
			DictObject_SetItem(frameobj->locals, BYTECODE->consts->item[oparg], STACK->item[frameobj->argindex--]);
			ListObject_ListDelItem(STACK, STACK->size - 1);
			break;
		}
		case OP_BUILD_LIST:
		{
			struct ListObject* list = ListObject_New();
			for (int i = 0; i < oparg; i++)
			{
				ListObject_InsertItem(list, 0, STACK->item[STACK->size - 1]);
				ListObject_ListDelItem(STACK, STACK->size - 1);
			}
			ListObject_InsertItem(STACK, STACK->size, list);
			break;
		}
		case OP_LOAD_SUB:
		{
			struct Object* value = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* slice= STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			//TODO
			ListObject_InsertItem(STACK, STACK->size, value->objattr->obj_getitem(value,slice));
			break;
		}
		case OP_STORE_SUB:
		{
			struct Object* value = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* slice = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* setvalue = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			//TODO
			value->objattr->obj_setitem(value, slice,setvalue);
			break;
		}
		case OP_LOAD_ATTR:
		{
			struct Object* attr = BYTECODE->consts->item[oparg];
			struct Object* value= STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(STACK, STACK->size, value->objattr->obj_getattr(value, attr));
			break;
		}
		case OP_STORE_ATTR:
		{
			struct Object* attr = BYTECODE->consts->item[oparg];
			struct Object* value = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* setvalue = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			value->objattr->obj_setattr(value, attr, setvalue);
			break;
		}
		case OP_SET_TEMP:
		{
			temp = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			break;
		}
		case OP_GET_TEMP:
		{
			ListObject_InsertItem(STACK, STACK->size,temp);
			break;
		}
		case OP_POP_TOP:
			ListObject_ListDelItem(STACK, STACK->size - 1);
			break;
		case OP_NEW_BLOCK:
		{
			ListObject_InsertItem(blocks, blocks->size, BlockObject_New());
			struct BlockObject* block = blocks->item[blocks->size - 1];
			ListObject_InsertItem(block->flags, block->flags->size, IntObject_NewWithValue(i));
			break;
		}
		case OP_DEL_BLOCK:
			ListObject_ListDelItem(blocks, blocks->size - 1);
			break;
		case OP_ADD_FLAG:
		{
			struct BlockObject* block = blocks->item[blocks->size - 1];
			ListObject_InsertItem(block->flags, block->flags->size, IntObject_NewWithValue(oparg));
			break;
		}
		case OP_GOTO_FLAG:
		{
			struct BlockObject* block = blocks->item[blocks->size - 1];
			i = ((struct IntObject*)block->flags->item[0])->value+((struct IntObject*)block->flags->item[oparg])->value;
			break;
		}
		}
#ifndef NDEBUG
		printf("\n%d:", i);
		FrameObject_Print(frameobj);
#endif
		i++;
	}
	return IntObject_NewWithValue(0);
}

struct FunctionObject* make_function(struct FrameObject* frameobj,struct StringObject* name, struct ByteCodeObject* bytecode)
{
	struct FunctionObject* func = FunctionObject_New();
	func->name = name->string;
	func->bytecode = bytecode;
	func->frame = frameobj;
	return func;
}

struct Object* call(struct Object* f, struct ListObject* args)
{
	if (CHECK(f, "function"))
	{
		struct FunctionObject* func = (struct FunctionObject*)f;
		struct FrameObject* frameobj = FrameObject_NewWithByteCode(func->bytecode);
		frameobj->argindex = args->size - 1;
		for (int i = 0; i < args->size; i++)
		{
			ListObject_InsertItem(frameobj->statck, frameobj->statck->size, args->item[i]);
		}
		struct DictObject* dict = DictObject_New();
		struct FrameObject* frame = func->frame;
		for (int i = 0; i < frame->globals->size; i++)
		{
			DictObject_SetItem(dict, frame->globals->item[i].key, frame->globals->item[i].value);
		}
		for (int i = 0; i < frame->locals->size; i++)
		{
			DictObject_SetItem(dict, frame->locals->item[i].key, frame->locals->item[i].value);
		}
		frameobj->globals = dict;
		return Eval(frameobj);
	}
	else if (CHECK(f, "cfunciton"))
	{
		struct CFunctionObject* func = (struct CFunctionObject*)f;
		struct ListObject* cfuncargs = ListObject_New();

		for (int i = 0; i < args->size; i++)
		{
			ListObject_InsertItem(cfuncargs, 0, args->item[i]);
		}

		return func->func(cfuncargs);
	}
	return IntObject_New();
}