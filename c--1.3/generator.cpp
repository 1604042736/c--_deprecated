#include "generator.h"

void generator(Object* obj, SyntaxTree* syntaxtree)
{
	CodeObject* codeobj = (CodeObject*)obj;
	if (syntaxtree == NULL)return;
	if (syntaxtree->asttype == AT_SYNTAXTREE)
	{
		switch (syntaxtree->nodetype)
		{
		case NT_NAME:
		{
			ListObject* names = ((ListObject*)(codeobj->names));
			Object* name = StringObject_NewFromString(syntaxtree->child["id"]->str);
			int index = names->objattr->obj_finditem((codeobj->names), name);
			if (index==-1)
			{
				names->objattr->obj_additem(codeobj->names, name);
				index = names->list.size() - 1;
			}
			ListObject* code = ((ListObject*)(codeobj->code));
			if (syntaxtree->child["mode"]->str == "store")
			{
				Object* op = IntObject_NewFromInt(STORE_NAME);
				Object* arg = IntObject_NewFromInt(index);
				code->objattr->obj_additem(codeobj->code,op);
				code->objattr->obj_additem(codeobj->code, arg);
			}
			else if (syntaxtree->child["mode"]->str == "load")
			{
				Object* op = IntObject_NewFromInt(LOAD_NAME);
				Object* arg = IntObject_NewFromInt(index);
				code->objattr->obj_additem(codeobj->code, op);
				code->objattr->obj_additem(codeobj->code, arg);
			}
			break;
		}
		case NT_NUM:
		{
			ListObject* consts = ((ListObject*)(codeobj->consts));
			Object* name = IntObject_NewFromInt(syntaxtree->child["value"]->value);
			int index = consts->objattr->obj_finditem((codeobj->consts), name);
			if (index==-1)
			{
				consts->objattr->obj_additem(codeobj->consts, name);
				index = consts->list.size() - 1;
			}
			ListObject* code = ((ListObject*)(codeobj->code));
			Object* op = IntObject_NewFromInt(LOAD_CONST);
			Object* arg = IntObject_NewFromInt(index);
			code->objattr->obj_additem(codeobj->code, op);
			code->objattr->obj_additem(codeobj->code, arg);
			break;
		}
		case NT_STRING:
		{
			ListObject* consts = ((ListObject*)(codeobj->consts));
			Object* name = StringObject_NewFromString(syntaxtree->child["id"]->str);
			int index = consts->objattr->obj_finditem((codeobj->consts), name);
			if (index == -1)
			{
				consts->objattr->obj_additem(codeobj->consts, name);
				index = consts->list.size() - 1;
			}
			ListObject* code = ((ListObject*)(codeobj->code));
			Object* op = IntObject_NewFromInt(LOAD_CONST);
			Object* arg = IntObject_NewFromInt(index);
			code->objattr->obj_additem(codeobj->code, op);
			code->objattr->obj_additem(codeobj->code, arg);
			break;
		}
		case NT_OPERATION:
		{
			generator((Object*)codeobj, syntaxtree->child["left"]);
			generator((Object*)codeobj, syntaxtree->child["right"]);
			ListObject* code = ((ListObject*)(codeobj->code));
			Object* op = IntObject_NewFromInt(operatormap[syntaxtree->child["op"]->str]);
			code->objattr->obj_additem(codeobj->code, op);
			break;
		}
		case NT_CALL:
		{
			generator((Object*)codeobj, syntaxtree->child["args"]);
			generator((Object*)codeobj, syntaxtree->child["func"]);
			ListObject* code = ((ListObject*)(codeobj->code));
			Object* op = IntObject_NewFromInt(CALL);
			Object* arg = IntObject_NewFromInt(syntaxtree->child["args"]->list.size());
			code->objattr->obj_additem(codeobj->code, op);
			code->objattr->obj_additem(codeobj->code, arg);
			break;
		}
		case NT_PROGRAM:
			generator((Object*)codeobj, syntaxtree->child["body"]);
			break;
		case NT_ASSIGN:
		{
			generator((Object*)codeobj, syntaxtree->child["value"]);
			generator((Object*)codeobj, syntaxtree->child["targets"]);
			break;
		}
		case NT_CONDITION:
		{
			ListObject* code = ((ListObject*)(codeobj->code));
			generator((Object*)codeobj, syntaxtree->child["exp"]);
			int backindex = code->list.size();	//等会儿要重新在这里插入代码
			generator((Object*)codeobj, syntaxtree->child["body"]);
			/*跳过body*/
			Object* op = IntObject_NewFromInt(IFJMP);
			Object* arg = IntObject_NewFromInt(code->list.size() - backindex+2);	//默认跳跃后会遇到JMP
			code->objattr->obj_insertitem(codeobj->code, IntObject_NewFromInt(backindex), op);
			code->objattr->obj_insertitem(codeobj->code, IntObject_NewFromInt(backindex + 1), arg);
			backindex=code->list.size();	//等会儿要重新在这里插入代码
			generator((Object*)codeobj, syntaxtree->child["elses"]);
			/*跳过elses*/
			op= IntObject_NewFromInt(JMP);
			arg = IntObject_NewFromInt(code->list.size() - backindex);
			code->objattr->obj_insertitem(codeobj->code, IntObject_NewFromInt(backindex), op);
			code->objattr->obj_insertitem(codeobj->code, IntObject_NewFromInt(backindex + 1), arg);
			break;
		}
		case NT_LOOP:
		{
			ListObject* code = ((ListObject*)(codeobj->code));
			int jmpindex = code->list.size();
			generator((Object*)codeobj, syntaxtree->child["exp"]);
			int backindex = code->list.size();
			generator((Object*)codeobj, syntaxtree->child["body"]);
			/*跳过body*/
			Object* op = IntObject_NewFromInt(IFJMP);
			Object* arg = IntObject_NewFromInt(code->list.size() - backindex+2);
			code->objattr->obj_insertitem(codeobj->code, IntObject_NewFromInt(backindex), op);
			code->objattr->obj_insertitem(codeobj->code, IntObject_NewFromInt(backindex + 1), arg);
			/*回到exp*/
			op = IntObject_NewFromInt(JMP);
			arg = IntObject_NewFromInt(jmpindex - code->list.size()-2);
			code->objattr->obj_additem(codeobj->code,  op);
			code->objattr->obj_additem(codeobj->code,  arg);
			break;
		}
		case NT_FUNCTIONDEF:
		{
			ListObject* consts = ((ListObject*)(codeobj->consts));
			ListObject* names = ((ListObject*)(codeobj->names));
			ListObject* code = ((ListObject*)(codeobj->code));
			/*添加名称*/
			Object* name = StringObject_NewFromString(syntaxtree->child["name"]->str);
			int index = consts->objattr->obj_finditem((codeobj->consts), name);
			if (index == -1)
			{
				consts->objattr->obj_additem(codeobj->consts, name);
				index = consts->list.size() - 1;
			}
			int indexname = names->objattr->obj_finditem((codeobj->names), name);
			if (indexname == -1)
			{
				names->objattr->obj_additem(codeobj->names, name);
				indexname = names->list.size() - 1;
			}

			/*添加函数的codeobject*/
			Object* funccodeobj = CodeObject_New();
			generator(funccodeobj, syntaxtree->child["body"]);

			int indexfunc = consts->objattr->obj_finditem((codeobj->consts), funccodeobj);
			if (indexfunc == -1)
			{
				consts->objattr->obj_additem(codeobj->consts, funccodeobj);
				indexfunc = consts->list.size() - 1;
			}

			generator((Object*)codeobj, syntaxtree->child["args"]);
			

			/*加载object*/
			Object* op = IntObject_NewFromInt(LOAD_CONST);
			Object* arg = IntObject_NewFromInt(indexfunc);
			code->objattr->obj_additem(codeobj->code, op);
			code->objattr->obj_additem(codeobj->code, arg);

			/*加载名称*/
			op = IntObject_NewFromInt(LOAD_CONST);
			arg = IntObject_NewFromInt(index);
			code->objattr->obj_additem(codeobj->code, op);
			code->objattr->obj_additem(codeobj->code, arg);

			/*创建函数*/
			op = IntObject_NewFromInt(MAKE_FUNCTION);
			arg = IntObject_NewFromInt(syntaxtree->child["args"]->list.size());
			code->objattr->obj_additem(codeobj->code, op);
			code->objattr->obj_additem(codeobj->code, arg);

			op = IntObject_NewFromInt(STORE_NAME);
			arg= IntObject_NewFromInt(indexname);
			code->objattr->obj_additem(codeobj->code, op);
			code->objattr->obj_additem(codeobj->code, arg);
			break;
		}
		case NT_RETURN:
		{
			ListObject* code = ((ListObject*)(codeobj->code));
			generator((Object*)codeobj, syntaxtree->child["value"]);
			Object* op = IntObject_NewFromInt(RETURN);
			code->objattr->obj_additem(codeobj->code, op);
			break;
		}
		case NT_LIST:
		{
			ListObject* code = ((ListObject*)(codeobj->code));
			generator((Object*)codeobj, syntaxtree->child["list"]);
			Object* op = IntObject_NewFromInt(BUILD_LIST);
			Object* arg = IntObject_NewFromInt(syntaxtree->child["list"]->list.size());
			code->objattr->obj_additem(codeobj->code, op);
			code->objattr->obj_additem(codeobj->code, arg);
			break;
		}
		case NT_SUBSCRIPT:
		{
			ListObject* code = ((ListObject*)(codeobj->code));
			generator((Object*)codeobj, syntaxtree->child["value"]);
			generator((Object*)codeobj, syntaxtree->child["slice"]);
			if (syntaxtree->child["mode"]->str == "store")
			{
				Object* op = IntObject_NewFromInt(STORE_SUBSCRIPT);
				code->objattr->obj_additem(codeobj->code, op);
			}
			else if (syntaxtree->child["mode"]->str == "load")
			{
				Object* op = IntObject_NewFromInt(LOAD_SUBSCRIPT);
				code->objattr->obj_additem(codeobj->code, op);
			}
			break;
		}
		}
	}
	else if (syntaxtree->asttype == AT_LIST)
	{
		for (int i = 0; i < syntaxtree->list.size(); i++)generator((Object*)codeobj, syntaxtree->list[i]);
	}
}

bool searchname(vector<Object*> list,string name)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i]->objattr->obj_name == "String" && ((StringObject*)list[i])->str == name)return true;
	}
	return false;
}