#include "astobject.h"

struct Object* NameSpaceASTObject_New()
{
	struct NameSpaceASTObject* astobject = (struct NameSpaceASTObject*)malloc(sizeof(struct NameSpaceASTObject));
	astobject->objattr = &NameSpaceASTObjectAttribute;
	astobject->lineno = 0;
	astobject->linepos = 0;
	astobject->body = ListObject_New();
	return (struct Object*)astobject;
}

struct Object* NameSpaceASTObject_NewWithParser(struct Parser* parser)
{
	struct NameSpaceASTObject* astobject = NameSpaceASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* IfASTObject_New()
{
	struct IfASTObject* astobject = (struct IfASTObject*)malloc(sizeof(struct IfASTObject));
	astobject->objattr = &IfASTObjectAttribute;
	astobject->lineno = 0;
	astobject->linepos = 0;
	astobject->body = ListObject_New();
	astobject->elses = ListObject_New();
	return (struct Object*)astobject;
}

struct Object* IfASTObject_NewWithParser(struct Parser* parser)
{
	struct IfASTObject* astobject = IfASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* OperationASTObject_New()
{
	struct OperationASTObject* astobject = (struct OperationASTObject*)malloc(sizeof(struct OperationASTObject));
	astobject->objattr = &OperationASTObjectAttribute;
	astobject->op = StringObject_New();
	return (struct Object*)astobject;
}

struct Object* OperationASTObject_NewWithParser(struct Parser* parser)
{
	struct OperationASTObject* astobject = OperationASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* NameASTObject_New()
{
	struct NameASTObject* astobject = (struct NameASTObject*)malloc(sizeof(struct NameASTObject));
	astobject->objattr = &NameASTObjectAttribute;
	astobject->id = StringObject_New();
	astobject->mode = StringObject_New();
	return (struct Object*)astobject;
}

struct Object* NameASTObject_NewWithParser(struct Parser* parser)
{
	struct NameASTObject* astobject = NameASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* ConstantASTObject_New()
{
	struct ConstantASTObject* astobject = (struct ConstantASTObject*)malloc(sizeof(struct ConstantASTObject));
	astobject->objattr = &ConstantASTObjectAttribute;
	return (struct Object*)astobject;
}

struct Object* ConstantASTObject_NewWithParser(struct Parser* parser)
{
	struct ConstantASTObject* astobject = ConstantASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* AssignASTObject_New()
{
	struct AssignASTObject* astobject = (struct AssignASTObject*)malloc(sizeof(struct AssignASTObject));
	astobject->objattr = &AssignASTObjectAttribute;
	astobject->targets = ListObject_New();
	return (struct Object*)astobject;
}

struct Object* AssignASTObject_NewWithParser(struct Parser* parser)
{
	struct AssignASTObject* astobject = AssignASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* SubscriptASTObject_New()
{
	struct SubscriptASTObject* astobject = (struct SubscriptASTObject*)malloc(sizeof(struct SubscriptASTObject));
	astobject->objattr = &SubscriptASTObjectAttribute;
	astobject->mode = StringObject_New();
	return (struct Object*)astobject;
}

struct Object* SubscriptASTObject_NewWithParser(struct Parser* parser)
{
	struct SubscriptASTObject* astobject = SubscriptASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* CallASTObject_New()
{
	struct CallASTObject* astobject = (struct CallASTObject*)malloc(sizeof(struct CallASTObject));
	astobject->objattr = &CallASTObjectAttribute;
	astobject->args = ListObject_New();
	return (struct Object*)astobject;
}

struct Object* CallASTObject_NewWithParser(struct Parser* parser)
{
	struct CallASTObject* astobject = CallASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* ReturnASTObject_New()
{
	struct ReturnASTObject* astobject = (struct ReturnASTObject*)malloc(sizeof(struct ReturnASTObject));
	astobject->objattr = &ReturnASTObjectAttribute;
	return (struct Object*)astobject;
}

struct Object* ReturnASTObject_NewWithParser(struct Parser* parser)
{
	struct ReturnASTObject* astobject = ReturnASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* FunctionDefASTObject_New()
{
	struct FunctionDefASTObject* astobject = (struct FunctionDefASTObject*)malloc(sizeof(struct FunctionDefASTObject));
	astobject->objattr = &FunctionDefASTObjectAttribute;
	astobject->name = StringObject_New();
	return (struct Object*)astobject;
}

struct Object* FunctionDefASTObject_NewWithParser(struct Parser* parser)
{
	struct ReturnASTObject* astobject = FunctionDefASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* WhileASTObject_New()
{
	struct WhileASTObject* astobject = (struct WhileASTObject*)malloc(sizeof(struct WhileASTObject));
	astobject->objattr = &WhileASTObjectAttribute;
	astobject->body = ListObject_New();
	return (struct Object*)astobject;
}

struct Object* WhileASTObject_NewWithParser(struct Parser* parser)
{
	struct WhileASTObject* astobject = WhileASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* ListASTObject_New()
{
	struct ListASTObject* astobject = (struct ListASTObject*)malloc(sizeof(struct ListASTObject));
	astobject->objattr = &ListASTObjectAttribute;
	astobject->list = ListObject_New();
	return (struct Object*)astobject;
}

struct Object* ListASTObject_NewWithParser(struct Parser* parser)
{
	struct ListASTObject* astobject = ListASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* AttributeASTObject_New()
{
	struct AttributeASTObject* astobject = (struct AttributeASTObject*)malloc(sizeof(struct AttributeASTObject));
	astobject->objattr = &AttributeASTObjectAttribute;
	return (struct Object*)astobject;
}

struct Object* AttributeASTObject_NewWithParser(struct Parser* parser)
{
	struct AttributeASTObject* astobject = AttributeASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* BreakASTObject_New()
{
	struct BreakASTObject* astobject = (struct BreakASTObject*)malloc(sizeof(struct BreakASTObject));
	astobject->objattr = &BreakASTObjectAttribute;
	return (struct Object*)astobject;
}

struct Object* BreakASTObject_NewWithParser(struct Parser* parser)
{
	struct BreakASTObject* astobject = BreakASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

struct Object* ContinueASTObject_New()
{
	struct ContinueASTObject* astobject = (struct ContinueASTObject*)malloc(sizeof(struct ContinueASTObject));
	astobject->objattr = &ContinueASTObjectAttribute;
	return (struct Object*)astobject;
}

struct Object* ContinueASTObject_NewWithParser(struct Parser* parser)
{
	struct ContinueASTObject* astobject = ContinueASTObject_New();
	astobject->lineno = parser->lexer->lineno;
	astobject->linepos = parser->lexer->linepos;
	return (struct Object*)astobject;
}

void printastobject(struct Object* obj,int space)
{
	if (obj == NULL)
	{
		printf("\n");
		return;
	}
	printSpace(space);
	if (CHECK(obj, "Namespace"))
	{
		struct NameSpaceASTObject* astobject = (struct NameSpaceASTObject*)obj;
		printf("Namespace\n");
		printSpace(space+1);
		printf("body\n");
		for (int i = 0; i < astobject->body->size; i++)
		{
			printastobject(astobject->body->item[i], space + 2);
		}
	}
	else if (CHECK(obj, "If"))
	{
		struct IfASTObject* astobject = (struct IfASTObject*)obj;
		printf("If\n");

		printSpace(space + 1);
		printf("exp\n");
		printastobject(astobject->exp, space + 2);

		printSpace(space + 1);
		printf("body\n");
		for (int i = 0; i < astobject->body->size; i++)
		{
			printastobject(astobject->body->item[i], space + 2);
		}

		printSpace(space + 1);
		printf("elses\n");
		for (int i = 0; i < astobject->elses->size; i++)
		{
			printastobject(astobject->elses->item[i], space + 2);
		}
	}
	else if (CHECK(obj, "Operation"))
	{
		struct OperationASTObject* astobject = (struct OperationASTObject*)obj;
		printf("Operation\n");

		printSpace(space + 1);
		printf("left\n");
		printastobject(astobject->left, space + 2);

		printSpace(space + 1);
		printf("op: ");
		printf("%s\n", astobject->op->string);

		printSpace(space + 1);
		printf("right\n");
		printastobject(astobject->right, space + 2);
	}
	else if (CHECK(obj, "Name"))
	{
		struct NameASTObject* astobject = (struct NameASTObject*)obj;
		printf("Name\n");

		printSpace(space + 1);
		printf("id: ");
		printf("%s\n", astobject->id->string);

		printSpace(space + 1);
		printf("mode: ");
		printf("%s\n", astobject->mode->string);
	}
	else if (CHECK(obj, "Constant"))
	{
		struct ConstantASTObject* astobject = (struct ConstantASTObject*)obj;
		printf("Constant\n");

		printSpace(space + 1);
		printf("value: ");
		astobject->value->objattr->obj_print(astobject->value);
		printf("\n");
	}
	else if (CHECK(obj, "Assign"))
	{
		struct AssignASTObject* astobject = (struct AssignASTObject*)obj;
		printf("Assign\n");

		printSpace(space + 1);
		printf("targets\n");
		for (int i = 0; i < astobject->targets->size; i++)
		{
			printastobject(astobject->targets->item[i], space + 2);
		}

		printSpace(space + 1);
		printf("value\n");
		printastobject(astobject->value, space + 2);
	}
	else if (CHECK(obj, "Subscript"))
	{
		struct SubscriptASTObject* astobject = (struct SubscriptASTObject*)obj;
		printf("Subscript\n");

		printSpace(space + 1);
		printf("value\n");
		printastobject(astobject->value, space + 2);

		printSpace(space + 1);
		printf("slice\n");
		printastobject(astobject->slice, space + 2);

		printSpace(space + 1);
		printf("mode: ");
		printf("%s\n", astobject->mode->string);
	}
	else if (CHECK(obj, "Call"))
	{
		struct CallASTObject* astobject = (struct CallASTObject*)obj;
		printf("Call\n");

		printSpace(space + 1);
		printf("func\n");
		printastobject(astobject->func, space + 2);

		printSpace(space + 1);
		printf("args\n");
		for (int i = 0; i < astobject->args->size; i++)
		{
			printastobject(astobject->args->item[i], space + 2);
		}
	}
	else if (CHECK(obj, "Return"))
	{
		struct ReturnASTObject* astobject = (struct ReturnASTObject*)obj;
		printf("Return\n");

		printSpace(space + 1);
		printf("value\n");
		printastobject(astobject->value, space + 2);
	}
	else if (CHECK(obj, "FunctionDef"))
	{
		struct FunctionDefASTObject* astobject = (struct FunctionDefASTObject*)obj;
		printf("FunctionDef\n");

		printSpace(space + 1);
		printf("name: ");
		printf("%s\n", astobject->name->string);

		printSpace(space + 1);
		printf("args\n");
		for (int i = 0; i < astobject->args->size; i++)
		{
			printastobject(astobject->args->item[i], space + 2);
		}

		printSpace(space + 1);
		printf("body\n");
		for (int i = 0; i < astobject->body->size; i++)
		{
			printastobject(astobject->body->item[i], space + 2);
		}
	}
	else if (CHECK(obj, "While"))
	{
		struct WhileASTObject* astobject = (struct WhileASTObject*)obj;
		printf("While\n");

		printSpace(space + 1);
		printf("exp\n");
		printastobject(astobject->exp, space + 2);

		printSpace(space + 1);
		printf("body\n");
		for (int i = 0; i < astobject->body->size; i++)
		{
			printastobject(astobject->body->item[i], space + 2);
		}
	}
	else if (CHECK(obj, "List"))
	{
		struct ListASTObject* astobject = (struct ListASTObject*)obj;
		printf("List\n");

		printSpace(space + 1);
		printf("list\n");
		for (int i = 0; i < astobject->list->size; i++)
		{
			printastobject(astobject->list->item[i], space + 2);
		}
	}
	else if (CHECK(obj, "Attribute"))
	{
		struct AttributeASTObject* astobject = (struct AttributeASTObject*)obj;
		printf("Attribute\n");

		printSpace(space + 1);
		printf("value\n");
		printastobject(astobject->value, space + 2);

		printSpace(space + 1);
		printf("attr: ");
		printf("%s\n", astobject->attr->string);

		printSpace(space + 1);
		printf("mode: ");
		printf("%s\n", astobject->mode->string);
	}
	else if (CHECK(obj, "Break"))
	{
		printf("Break\n");
	}
	else if (CHECK(obj, "Continue"))
	{
		printf("Continue\n");
	}
}