#include "eval.h"

Object* frame_eval(Object* obj)
{
	FrameObject* frameobj = (FrameObject*)obj;
#ifndef NDEBUG
	cout << "-->"<<obj << endl;
	print(frameobj->builtins);
	cout << endl;
	print(frameobj->globals);
	cout << endl;
#endif
	CodeObject* codeobj = (CodeObject*)(frameobj->codeobj);
	ListObject* code = (ListObject*)codeobj->code;
	ListObject* names = (ListObject*)codeobj->names;
	ListObject* consts = (ListObject*)codeobj->consts;

	Object* result;
	Object* arg;
	IntObject* op;
	int code_pos = 0;	//代码所在位置
	int codesize = code->list.size();
	while (code_pos < codesize)
	{
		op = (IntObject*)code->list[code_pos];
		switch (op->val)
		{
		case LOAD_NAME:
		{
			arg = code->list[++code_pos];
			Object* name = ListObject_GetItem(codeobj->names, arg);
			/*先到local中查找*/
			Object* val = DictObject_GetItem(frameobj->locals, name);
			if (val == NULL)
			{
				/*到global中查找*/
				val = DictObject_GetItem(frameobj->globals, name);
				if (val == NULL)
				{
					/*到builtins中查找*/
					val = DictObject_GetItem(frameobj->builtins, name);
					if (val == NULL)
					{
						//TODO 抛出异常
						cout << "不存在的key: ";
						print(name);
						exit(-1);
					}
				}
			}
			PUSHVAL(frameobj, val);
			break;
		}
		case LOAD_CONST:
		{
			arg = code->list[++code_pos];
			PUSHCONST(frameobj, arg);
			break;
		}
		case STORE_NAME:
		{
			arg = code->list[++code_pos];
			TOP(name, frameobj->stack);
			DictObject_DictAddItem(frameobj->locals, ListObject_GetItem(codeobj->names, arg), name);
			break;
		}
		case ADD:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if (one->objattr->obj_add)
			{
				PUSH(frameobj, one->objattr->obj_add(one, two));
			}
			break;
		}
		case SUB:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if (one->objattr->obj_sub)
			{
				PUSH(frameobj, one->objattr->obj_sub(one, two));
			}
			break;
		}
		case MUL:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if(one->objattr->obj_mul)
			{
				PUSH(frameobj, one->objattr->obj_mul(one, two));
			}
			break;
		}
		case DIV:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if(one->objattr->obj_div)
			{
				PUSH(frameobj, one->objattr->obj_div(one, two));
			}
			break;
		}
		case MOD:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if(one->objattr->obj_mod)
			{
				PUSH(frameobj, one->objattr->obj_mod(one, two));
			}
			break;
		}
		case EQ:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if(one->objattr->obj_eq)
			{
				result = IntObject_NewFromInt(one->objattr->obj_eq(one, two));
				PUSH(frameobj, result);
			}
			break;
		}
		case OR :
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if (one->objattr->obj_or)
			{
				result = IntObject_NewFromInt(one->objattr->obj_or(one, two));
				PUSH(frameobj, result);
			}
			break;
		}
		case GT:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if (one->objattr->obj_gt)
			{
				result = IntObject_NewFromInt(one->objattr->obj_gt(one, two));
				PUSH(frameobj, result);
			}
			break;
		}
		case GEQ:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if (one->objattr->obj_geq)
			{
				result = IntObject_NewFromInt(one->objattr->obj_geq(one, two));
				PUSH(frameobj, result);
			}
			break;
		}
		case LT:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if (one->objattr->obj_lt)
			{
				result = IntObject_NewFromInt(one->objattr->obj_lt(one, two));
				PUSH(frameobj, result);
			}
			break;
		}
		case LEQ:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if (one->objattr->obj_leq)
			{
				result = IntObject_NewFromInt(one->objattr->obj_leq(one, two));
				PUSH(frameobj, result);
			}
			break;
		}
		case NEQ:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			result = IntObject_NewFromInt(one->objattr->obj_neq(one, two));
			PUSH(frameobj, result);
			break;
		}
		case AND:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			if (one->objattr->obj_and)
			{
				result = IntObject_NewFromInt(one->objattr->obj_and(one, two));
				PUSH(frameobj, result);
			}
			break;
		}
		case IFJMP:
		{
			arg = code->list[++code_pos];
			TOP(val, frameobj->stack);
			if (val->objattr->obj_bool&&!val->objattr->obj_bool(val))
			{
				code_pos += ((IntObject*)arg)->val;
			}
			break;
		}
		case JMP:
		{
			arg = code->list[++code_pos];
			code_pos += ((IntObject*)arg)->val;
			break;
		}
		case LOAD_VALUE:
		{
			arg = code->list[++code_pos];
			PUSH(frameobj, arg);
			break;
		}
		case RETURN:
		{
			TOP(val, frameobj->stack);
			return val;
		}
		case MAKE_FUNCTION:
		{
			arg = code->list[++code_pos];
			TOP(name, frameobj->stack);
			TOP(funccodeobj, frameobj->stack);
			Object* funcargname = ListObject_New();
			for (int i = 0; i < ((IntObject*)arg)->val; i++)
			{
				TOP(funcarg, frameobj->stack);
				((ListObject*)funcargname)->list.push_back(funcarg);
			}
			Object* funcobj = make_function((Object*)frameobj, name, funccodeobj, funcargname);
			PUSH(frameobj, funcobj);
			break;
		}
		case CALL:
		{
			arg = code->list[++code_pos];
			TOP(funcobj, frameobj->stack);
			Object* funcargs = ListObject_New();
			for (int i = 0; i < ((IntObject*)arg)->val; i++)
			{
				TOP(funcarg, frameobj->stack);
				((ListObject*)funcargs)->list.push_back(funcarg);
			}
			Object* returnval=call_function((Object*)frameobj,funcobj, funcargs);
			PUSH(frameobj, returnval);
			break;
		}
		case BUILD_LIST:
		{
			arg = code->list[++code_pos];
			ListObject* list = (ListObject*)ListObject_New();
			for (int i = 0; i < ((IntObject*)arg)->val; i++)
			{
				TOP(val, frameobj->stack);
				((ListObject*)list)->list.insert(((ListObject*)list)->list.begin(),val);
			}
			PUSH(frameobj, (Object*)list);
			break;
		}
		case LOAD_SUBSCRIPT:
		{
			TOP(slice, frameobj->stack);
			TOP(val, frameobj->stack);
			Object* sub = val->objattr->obj_getitem(val, slice);
			PUSH(frameobj, sub);
			break;
		}
		case STORE_SUBSCRIPT:
		{
			TOP(slice, frameobj->stack);
			TOP(val, frameobj->stack);
			TOP(assignval, frameobj->stack);
			val->objattr->obj_setitem(val, slice,assignval);
			break;
		}
		case POP_TOP:
		{
			TOP(val, frameobj->stack);
			break;
		}
		case LOAD_ATTR:
		{
			arg = code->list[++code_pos];
			Object* attr=ListObject_GetItem(codeobj->consts, arg);
			TOP(value, frameobj->stack);
			PUSH(frameobj, DictObject_GetItem(value->objattr->dict,attr));
			break;
		}
		case STORE_ATTR:
		{
			arg = code->list[++code_pos];
			Object* attr = ListObject_GetItem(codeobj->consts, arg);
			TOP(value, frameobj->stack);
			TOP(assignvalue, frameobj->stack);
			DictObject_DictAddItem(value->objattr->dict, attr, assignvalue);
			break;
		}
		}
#ifndef NDEBUG
		cout << op->val;
		print(frameobj->stack);
		print(frameobj->locals);
		cout << endl;
#endif
		code_pos++;
	}
	return NULL;
}

Object* make_function(Object* frameobj, Object* name, Object* funccodeobj,Object* funcargnames)
{
	FunctionObject* funcobj = (FunctionObject*)FunctionObject_New();
	funcobj->name = name;
	funcobj->frame = FrameObject_NewFrameCodeObject(funccodeobj,((FrameObject*)frameobj)->builtins);
	((FrameObject*)funcobj->frame)->globals = ((FrameObject*)frameobj)->locals;
	funcobj->argnames = funcargnames;
	return (Object*)funcobj;
}

Object* call_function(Object* frameobj, Object* obj, Object* args)
{
	if (obj->objattr->obj_name == "Function")
	{
		/*拷贝函数对象,防止调用的时候干扰到其他函数的locals*/
		FunctionObject* funcobj = (FunctionObject*)obj->objattr->obj_copy(obj);
		ListObject* argslist = (ListObject*)args;
		ListObject* argnamelist = (ListObject*)funcobj->argnames;
		int listsize = argslist->list.size();
		for (int i = 0; i < listsize; i++)
		{
			DictObject_DictAddItem(((FrameObject*)funcobj->frame)->locals, argnamelist->list[i], argslist->list[i]);
		}
		Object* val = frame_eval(funcobj->frame);
		return val;
	}
	else if (obj->objattr->obj_name == "CFunction")
	{
		CFunctionObject* cfuncobj = (CFunctionObject*)obj;
		if (cfuncobj->self != NULL)
		{
			ListObject_InsertItem(args, IntObject_NewFromInt(0), cfuncobj->self);
		}
		return cfuncobj->func(args);
	}
	return IntObject_NewFromInt(0);
}