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
	unsigned short op;
	short oparg = 0;
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
		oparg = index;
		APPENDOPCODE;
	}
	else if (CHECK(obj, "Num"))
	{
		struct NumASTObject* astobject = (struct NumASTObject*)obj;
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
		
	}
	else if (CHECK(obj, "Call"))
	{
		struct CallASTObject* astobject = (struct CallASTObject*)obj;
		Translater_Translate(translater, astobject->args);
		Translater_Translate(translater, astobject->func);
		op = OP_CALL;
		oparg = astobject->args->size;
		APPENDOPCODE;
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
}