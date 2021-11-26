#include "eval.h"

struct Object* Eval(struct FrameObject* frameobj)
{
#define BYTECODE	(frameobj->bytecode)
#define CODE		(BYTECODE->code)
#define STACK		(frameobj->statck)
	int i = 0,op,oparg,index,lineno,linepos;
	int size = frameobj->bytecode->code->size;
	struct ListObject* temp=ListObject_New();
	struct ListObject* blocks = ListObject_New();
	struct StringObject* filename;
	while (i < size)
	{
		op = ((struct OpCodeObject*)frameobj->bytecode->code->item[i])->op;
		oparg= ((struct OpCodeObject*)frameobj->bytecode->code->item[i])->oparg;
		lineno=((struct OpCodeObject*)frameobj->bytecode->code->item[i])->lineno;
		linepos = ((struct OpCodeObject*)frameobj->bytecode->code->item[i])->linepos;
		filename = ((struct OpCodeObject*)frameobj->bytecode->code->item[i])->filename;
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
					exception = ExceptionObject_NewWithMessage(StringObject_NewWithString("找不到"),filename, lineno, linepos);
					goto ERROR;
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
			struct Object* result = Object_Add(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_SUB:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Sub(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_MUL:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Mul(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_DIV:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Div(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_MOD:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Mod(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_EQ:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Eq(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_NEQ:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Neq(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_GT:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Gt(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_GEQ:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Geq(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_LT:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Lt(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_LEQ:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Leq(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_AND:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_And(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
			break;
		}
		case OP_OR:
		{
			struct Object* right = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* left = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			struct Object* result = Object_Or(left, right);
			if (result == NULL)
			{
				goto ERROR;
			}
			ListObject_InsertItem(STACK, STACK->size, result);
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
			struct Object* t = STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			ListObject_InsertItem(temp, temp->size, t);
			break;
		}
		case OP_GET_TEMP:
		{
			ListObject_InsertItem(STACK, STACK->size,temp->item[temp->size-1]);
			break;
		}
		case OP_POP_TEMP:
			ListObject_ListDelItem(temp, temp->size - 1);
			break;
		case OP_POP_TOP:
			ListObject_ListDelItem(STACK, STACK->size - 1);
			break;
		case OP_NEW_BLOCK:
		{
			ListObject_InsertItem(blocks, blocks->size, BlockObject_New());
			struct BlockObject* block = blocks->item[blocks->size - 1];
			block->type = oparg;
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
		case OP_LOAD_NAMESPACE:
		{
			struct Object* name = BYTECODE->consts->item[oparg];
			struct NamespaceObject* nt = load_namespace(name);
			ListObject_InsertItem(STACK, STACK->size, nt);
			break;
		}
		case OP_LOAD_LOCAL:
			ListObject_InsertItem(STACK, STACK->size, frameobj->locals);
			break;
		case OP_BUILD_CLASS:
		{
			struct ClassObject* classobject= ClassObject_New();
			struct DictObject* attr= STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);
			for (int i = 0; i < oparg; i++)
			{
				ListObject_InsertItem(classobject->bases, 0, STACK->item[STACK->size - 1]);
				ListObject_ListDelItem(STACK, STACK->size - 1);
			}
			classobject->name= STACK->item[STACK->size - 1];
			ListObject_ListDelItem(STACK, STACK->size - 1);

			for (int i = 0; i < attr->size; i++)
			{
				DictObject_SetItem(classobject->dict, attr->item[i].key, attr->item[i].value);
			}

			ListObject_InsertItem(STACK, STACK->size, classobject);
			break;
		}
		}
#ifndef NDEBUG
		printf("\n%d:", i);
		FrameObject_Print(frameobj);
#endif
		i++;
		continue;
	ERROR:
		{
			struct BlockObject* block = blocks->item[blocks->size - 1];
			if (blocks->size - 1>=0&&block!=NULL&&block->type == BLOCKTYPE_EXCEPTION)
			{
				i = ((struct IntObject*)block->flags->item[0])->value + ((struct IntObject*)block->flags->item[1])->value+1;
				ListObject_ListDelItem(blocks, blocks->size - 1);
			}
			else if (exception != NULL)
			{
				printf("\n错误:文件 %s:行 %d,列 %d:", filename->string,lineno, linepos);
				printf("%s\n", exception->message->string);
				exit(-1);
			}
		}
	}
	free(temp);
	free(blocks);
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
	return f->objattr->obj_call(f, args);
}

struct NamespaceObject* load_namespace(struct StringObject* name)
{
	struct StringObject* filename = name;
	filename = StringObject_Add(filename, StringObject_NewWithString(".c--"));
	return compiler(filename->string,name->string);
}