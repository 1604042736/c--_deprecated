#include "translater.h"

struct Translater* Translater_New()
{
	struct Translater* tr = (struct Translater*)malloc(sizeof(struct Translater));
	tr->bytecode = ByteCodeObject_New();
	return tr;
}

void Translater_Init()
{
	operationmap = DictObject_New();
	DictObject_SetItem(operationmap, StringObject_NewWithString("+"), IntObject_NewWithValue(OP_ADD));
	DictObject_SetItem(operationmap, StringObject_NewWithString("-"), IntObject_NewWithValue(OP_SUB));
	DictObject_SetItem(operationmap, StringObject_NewWithString("*"), IntObject_NewWithValue(OP_MUL));
	DictObject_SetItem(operationmap, StringObject_NewWithString("/"), IntObject_NewWithValue(OP_DIV));
	DictObject_SetItem(operationmap, StringObject_NewWithString("%"), IntObject_NewWithValue(OP_MOD));
	DictObject_SetItem(operationmap, StringObject_NewWithString("=="), IntObject_NewWithValue(OP_EQ));
	DictObject_SetItem(operationmap, StringObject_NewWithString("!="), IntObject_NewWithValue(OP_NEQ));
	DictObject_SetItem(operationmap, StringObject_NewWithString(">"), IntObject_NewWithValue(OP_GT));
	DictObject_SetItem(operationmap, StringObject_NewWithString(">="), IntObject_NewWithValue(OP_GEQ));
	DictObject_SetItem(operationmap, StringObject_NewWithString("<"), IntObject_NewWithValue(OP_LT));
	DictObject_SetItem(operationmap, StringObject_NewWithString("<="), IntObject_NewWithValue(OP_LEQ));
	DictObject_SetItem(operationmap, StringObject_NewWithString("or"), IntObject_NewWithValue(OP_OR));
	DictObject_SetItem(operationmap, StringObject_NewWithString("and"), IntObject_NewWithValue(OP_AND));
}

void Translater_Translate(struct Translater* translater,struct Object* obj)
{
#define APPENDOPCODE		ListObject_InsertItem(translater->bytecode->code, translater->bytecode->code->size, OpCodeObject_NewWithOpCodeAndLine(op, oparg, astobject->lineno, astobject->linepos))
#define INSERTOPCODE(INDEX)	ListObject_InsertItem(translater->bytecode->code, INDEX, OpCodeObject_NewWithOpCodeAndLine(op, oparg, astobject->lineno, astobject->linepos))
	if (obj == NULL)return;
	opcodetype op, oparg=0;
	if (CHECK(obj, "list"))
	{
		struct ListObject* list = (struct ListObject*)obj;
		for (int i = 0; i < list->size; i++)
		{
			Translater_Translate(translater, list->item[i]);
		}
	}
	else if (CHECK(obj, "Namespace"))
	{
		struct NameSpaceASTObject* astobject = (struct NameSpaceASTObject*)obj;
		Translater_Translate(translater, astobject->body);
	}
	else if (CHECK(obj, "If"))
	{
		struct IfASTObject* astobject = (struct IfASTObject*)obj;
		Translater_Translate(translater, astobject->exp);
		int backindex = translater->bytecode->code->size;
		Translater_Translate(translater, astobject->body);
		op = OP_IFJMP;
		oparg = translater->bytecode->code->size-backindex+1;
		INSERTOPCODE(backindex);
		backindex = translater->bytecode->code->size;
		Translater_Translate(translater, astobject->elses);
		op = OP_JMP;
		oparg = translater->bytecode->code->size - backindex;
		INSERTOPCODE(backindex);

	}
	else if (CHECK(obj, "Operation"))
	{
		struct OperationASTObject* astobject = (struct OperationASTObject*)obj;
		Translater_Translate(translater, astobject->left);
		Translater_Translate(translater, astobject->right);
		struct IntObject* opobj = DictObject_GetItem(operationmap, astobject->op);
		op = opobj->value;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Name"))
	{
		struct NameASTObject* astobject = (struct NameASTObject*)obj;
		int index = ListObject_FindItem(translater->bytecode->consts, astobject->id);
		if (index == -1)
		{
			index = translater->bytecode->consts->size;
			ListObject_InsertItem(translater->bytecode->consts, translater->bytecode->consts->size, astobject->id);
		}
		if (!strcmp(astobject->mode->string, "load"))
		{
			op = OP_LOAD_NAME;
		}
		else if(!strcmp(astobject->mode->string,"store"))
		{
			op = OP_STORE_NAME;
		}
		else if (!strcmp(astobject->mode->string, "arg"))
		{
			op = OP_STORE_ARG;
		}
		else if (!strcmp(astobject->mode->string, "import"))
		{
			op = OP_LOAD_NAMESPACE;
		}
		oparg = index;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Constant"))
	{
		struct ConstantASTObject* astobject = (struct ConstantASTObject*)obj;
		int index = ListObject_FindItem(translater->bytecode->consts, astobject->value);
		if (index == -1)
		{
			index = translater->bytecode->consts->size;
			ListObject_InsertItem(translater->bytecode->consts, translater->bytecode->consts->size, astobject->value);
		}
		op = OP_LOAD_CONST;
		oparg = index;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Assign"))
	{
		struct AssignASTObject* astobject = (struct AssignASTObject*)obj;
		Translater_Translate(translater, astobject->value);
		Translater_Translate(translater, astobject->targets);
	}
	else if (CHECK(obj, "Subscript"))
	{
		struct SubscriptASTObject* astobject = (struct SubscriptASTObject*)obj;
		Translater_Translate(translater, astobject->slice);
		Translater_Translate(translater, astobject->value);

		if (!strcmp(astobject->mode->string, "load"))
		{
			op = OP_LOAD_SUB;
		}
		else
		{
			op = OP_STORE_SUB;
		}
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Call"))
	{
		struct CallASTObject* astobject = (struct CallASTObject*)obj;
		oparg = astobject->args->size;
		//TODO 改进
		if (CHECK(astobject->func, "Attribute"))
		{
			struct AttributeASTObject* func = (struct AttributeASTObject*)astobject->func;
			if (strcmp(func->mode->string, "namespace"))
			{
				Translater_Translate(translater, func->value);
				op = OP_SET_TEMP;
				APPENDOPCODE;
				op = OP_GET_TEMP;
				APPENDOPCODE;
			}
		}
		Translater_Translate(translater, astobject->args);
		if (CHECK(astobject->func, "Attribute"))
		{
			struct AttributeASTObject* func = (struct AttributeASTObject*)astobject->func;
			if (strcmp(func->mode->string, "namespace"))
			{
				int index = ListObject_FindItem(translater->bytecode->consts, func->attr);
				if (index == -1)
				{
					index = translater->bytecode->consts->size;
					ListObject_InsertItem(translater->bytecode->consts, translater->bytecode->consts->size, func->attr);
				}

				op = OP_GET_TEMP;
				APPENDOPCODE;

				if (!strcmp(func->mode->string, "load"))
				{
					op = OP_LOAD_ATTR;
				}
				else if (!strcmp(func->mode->string, "store"))
				{
					op = OP_STORE_ATTR;
				}
				oparg = index;
				APPENDOPCODE;
				oparg = astobject->args->size + 1;
			}
			else
			{
				Translater_Translate(translater, astobject->func);
			}
		}
		else
		{
			Translater_Translate(translater, astobject->func);
		}
		op = OP_CALL;
		APPENDOPCODE;

		if (!strcmp(astobject->mode->string, "sentence"))
		{
			op = OP_POP_TOP;
			APPENDOPCODE;
		}
	}
	else if (CHECK(obj, "Return"))
	{
		struct ReturnASTObject* astobject = (struct ReturnASTObject*)obj;
		Translater_Translate(translater, astobject->value);
		op = OP_RETURN;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "FunctionDef"))
	{
		struct FunctionDefASTObject* astobject = (struct FunctionDefASTObject*)obj;

		int index = ListObject_FindItem(translater->bytecode->consts, astobject->name);
		if (index == -1)
		{
			index = translater->bytecode->consts->size;
			ListObject_InsertItem(translater->bytecode->consts, translater->bytecode->consts->size, astobject->name);
		}
		op = OP_LOAD_CONST;
		oparg = index;
		APPENDOPCODE;

		struct Translater* tr = Translater_New();
		Translater_Translate(tr, astobject->args);
		Translater_Translate(tr, astobject->body);
		tr->bytecode->name = astobject->name->string;
		ListObject_InsertItem(translater->bytecode->consts, translater->bytecode->consts->size, tr->bytecode);
		op = OP_LOAD_CONST;
		oparg = translater->bytecode->consts->size - 1;
		APPENDOPCODE;

		op = OP_MAKE_FUNCTION;
		APPENDOPCODE;

		op = OP_STORE_NAME;
		oparg = index;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "While"))
	{
		struct WhileASTObject* astobject = (struct WhileASTObject*)obj;
		op = OP_NEW_BLOCK;
		APPENDOPCODE;
		op = OP_ADD_FLAG;
		/*设置continue的flag(while开始的地方)*/
		oparg= 2;
		APPENDOPCODE;
		int blockbackindex = translater->bytecode->code->size;

		int jmpindex = translater->bytecode->code->size;
		Translater_Translate(translater,astobject->exp);
		int backindex = translater->bytecode->code->size;
		Translater_Translate(translater, astobject->body);
		/*跳过body*/
		op = OP_IFJMP;
		oparg = translater->bytecode->code->size - backindex + 1;
		INSERTOPCODE(backindex);

		/*设置break的flag(while结束的地方)*/
		op = OP_ADD_FLAG;
		oparg= translater->bytecode->code->size - blockbackindex + 3;
		INSERTOPCODE(blockbackindex);
		/*回到exp*/
		op = OP_JMP;
		oparg = jmpindex - translater->bytecode->code->size - 1;
		APPENDOPCODE;

		op = OP_DEL_BLOCK;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "List"))
	{
		struct ListASTObject* astobject = (struct ListASTObject*)obj;
		Translater_Translate(translater, astobject->list);

		op = OP_BUILD_LIST;
		oparg = astobject->list->size;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Attribute"))
	{
		struct AttributeASTObject* astobject = (struct AttributeASTObject*)obj;

		int index = ListObject_FindItem(translater->bytecode->consts, astobject->attr);
		if (index == -1)
		{
			index = translater->bytecode->consts->size;
			ListObject_InsertItem(translater->bytecode->consts, translater->bytecode->consts->size, astobject->attr);
		}

		Translater_Translate(translater, astobject->value);

		if (!strcmp(astobject->mode->string, "load"))
		{
			op = OP_LOAD_ATTR;
		}
		else if (!strcmp(astobject->mode->string, "store"))
		{
			op = OP_STORE_ATTR;
		}
		else if (!strcmp(astobject->mode->string, "namespace"))
		{
			op = OP_LOAD_ATTR;
		}
		oparg = index;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Break"))
	{
		struct BreakASTObject* astobject = (struct BreakASTObject*)obj;
		op = OP_GOTO_FLAG;
		oparg = 2;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Continue"))
	{
		struct ContinueASTObject* astobject = (struct ContinueASTObject*)obj;
		op = OP_GOTO_FLAG;
		oparg = 1;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Import"))
	{
		struct ImportASTObject* astobject = (struct ImportASTObject*)obj;
		Translater_Translate(translater, astobject->name);

		if (CHECK(astobject->name, "Name"))
		{
			struct NameASTObject* name = (struct NameASTObject*)astobject->name;
			int index = ListObject_FindItem(translater->bytecode->consts, name->id);
			if (index == -1)
			{
				index = translater->bytecode->consts->size;
				ListObject_InsertItem(translater->bytecode->consts, translater->bytecode->consts->size, name->id);
			}
			oparg = index;
		}
		else if (CHECK(astobject->name, "Attribute"))
		{
			struct AttributeASTObject* at = (struct AttributeASTObject*)astobject->name;
			int index = ListObject_FindItem(translater->bytecode->consts, at->attr);
			if (index == -1)
			{
				index = translater->bytecode->consts->size;
				ListObject_InsertItem(translater->bytecode->consts, translater->bytecode->consts->size, at->attr);
			}
			oparg = index;
		}

		op = OP_STORE_NAME;
		APPENDOPCODE;
	}
}