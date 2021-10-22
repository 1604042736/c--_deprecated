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
	int code_pos = 0;	//��������λ��
	while (code_pos < code->list.size())
	{
		IntObject* op = (IntObject*)code->list[code_pos];
		switch (op->val)
		{
		case LOAD_NAME:
		{
			Object* arg = code->list[++code_pos];
			Object* name = codeobj->names->objattr->obj_getitem(codeobj->names, arg);
			/*�ȵ�local�в���*/
			Object* val = frameobj->locals->objattr->obj_getitem(frameobj->locals, name);
			if (val == NULL)
			{
				/*��global�в���*/
				val = frameobj->globals->objattr->obj_getitem(frameobj->globals, name);
				if (val == NULL)
				{
					/*��builtins�в���*/
					val = frameobj->builtins->objattr->obj_getitem(frameobj->builtins, name);
					if (val == NULL)
					{
						//TODO �׳��쳣
						cout << "�����ڵ�key: ";
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
			Object* arg = code->list[++code_pos];
			PUSHCONST(frameobj, arg);
			break;
		}
		case STORE_NAME:
		{
			Object* arg = code->list[++code_pos];
			TOP(name, frameobj->stack);
			frameobj->locals->objattr->obj_dictadditem(frameobj->locals, names->objattr->obj_getitem(codeobj->names, arg), name);
			break;
		}
		case ADD:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			PUSH(frameobj, one->objattr->obj_add(one, two));
			break;
		}
		case SUB:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			PUSH(frameobj, one->objattr->obj_sub(one, two));
			break;
		}
		case MUL:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			PUSH(frameobj, one->objattr->obj_mul(one, two));
			break;
		}
		case DIV:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			PUSH(frameobj, one->objattr->obj_div(one, two));
			break;
		}
		case MOD:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			PUSH(frameobj, one->objattr->obj_mod(one, two));
			break;
		}
		case EQ:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			Object* result = IntObject_NewFromInt(one->objattr->obj_eq(one, two));
			PUSH(frameobj, result);
			break;
		}
		case OR :
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			Object* result = IntObject_NewFromInt(one->objattr->obj_or(one, two));
			PUSH(frameobj, result);
			break;
		}
		case GT:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			Object* result = IntObject_NewFromInt(one->objattr->obj_gt(one, two));
			PUSH(frameobj, result);
			break;
		}
		case GEQ:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			Object* result = IntObject_NewFromInt(one->objattr->obj_geq(one, two));
			PUSH(frameobj, result);
			break;
		}
		case LT:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			Object* result = IntObject_NewFromInt(one->objattr->obj_lt(one, two));
			PUSH(frameobj, result);
			break;
		}
		case LEQ:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			Object* result = IntObject_NewFromInt(one->objattr->obj_leq(one, two));
			PUSH(frameobj, result);
			break;
		}
		case NEQ:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			Object* result = IntObject_NewFromInt(one->objattr->obj_neq(one, two));
			PUSH(frameobj, result);
			break;
		}
		case AND:
		{
			TOP(two, frameobj->stack);
			TOP(one, frameobj->stack);
			Object* result = IntObject_NewFromInt(one->objattr->obj_and(one, two));
			PUSH(frameobj, result);
			break;
		}
		case IFJMP:
		{
			Object* arg = code->list[++code_pos];
			TOP(val, frameobj->stack);
			if (!val->objattr->obj_bool(val))
			{
				code_pos += ((IntObject*)arg)->val;
			}
			break;
		}
		case JMP:
		{
			Object* arg = code->list[++code_pos];
			code_pos += ((IntObject*)arg)->val;
			break;
		}
		case LOAD_VALUE:
		{
			Object* arg = code->list[++code_pos];
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
			Object* arg = code->list[++code_pos];
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
			Object* arg = code->list[++code_pos];
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
			Object* arg = code->list[++code_pos];
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
		}
		cout << op->val;
		print(frameobj->stack);
		print(frameobj->locals);
		cout << endl;
		code_pos++;
	}
	return NULL;
}

Object* make_function(Object* frameobj, Object* name, Object* funccodeobj,Object* funcargnames)
{
	FunctionObject* funcobj = (FunctionObject*)FunctionObject_New();
	funcobj->name = name;
	funcobj->frame = FrameObject_NewFrameCodeObject(funccodeobj);
	((FrameObject*)funcobj->frame)->globals = ((FrameObject*)frameobj)->locals;
	funcobj->argnames = funcargnames;
	return (Object*)funcobj;
}

Object* call_function(Object* frameobj, Object* obj, Object* args)
{
	if (obj->objattr->obj_name == "Function")
	{
		/*������������,��ֹ���õ�ʱ����ŵ�����������locals*/
		FunctionObject* funcobj = (FunctionObject*)obj->objattr->obj_copy(obj);
		ListObject* argslist = (ListObject*)args;
		ListObject* argnamelist = (ListObject*)funcobj->argnames;
		for (int i = 0; i < argslist->list.size(); i++)
		{
			((FrameObject*)funcobj->frame)->locals->objattr->obj_dictadditem(((FrameObject*)funcobj->frame)->locals, argnamelist->list[i], argslist->list[i]);
		}
		Object* val = frame_eval(funcobj->frame);
		return val;
	}
	else if (obj->objattr->obj_name == "CFunction")
	{
		CFunctionObject* cfuncobj = (CFunctionObject*)obj;
		return cfuncobj->func(args);
	}
	return IntObject_NewFromInt(0);
}