#include "parser.h"

struct Parser* Parser_New(struct Lexer* lexer)
{
	struct Parser* parser = (struct Parser*)malloc(sizeof(struct Parser));
	if (parser == NULL)
	{
		printf("parser分配内存失败");
		exit(-1);
	}
	parser->lexer = lexer;
	parser->blockstyle = INDENT;
	parser->importmode = 0;
	parser->namespace_names = DictObject_New();
	return parser;
}

struct Object* Parser_Parse(struct Parser* parser)
{
	struct NameSpaceASTObject* astobject = (struct NameSpaceASTObject*)NameSpaceASTObject_NewWithParser(parser);
	astobject->body = (struct ListObject*)Parser_body(parser,"",-1);
	return (struct Object*)astobject;
}

struct Object* Parser_body(struct Parser* parser,char* indent,int token)
{
	struct ListObject* t = ListObject_New();
	//TODO 不同的代码样式
	if (parser->blockstyle == BRACES)
	{
		while (parser->lexer->token != TK_EOF && parser->lexer->token != TK_RLARGE)
		{
			ListObject_InsertItem(t, t->size, Parser_sentence(parser));
		}
	}
	else if (parser->blockstyle == INDENT)
	{
		while (parser->lexer->token != TK_EOF&&!strcmp(indent,parser->lexer->indent->string))
		{
			ListObject_InsertItem(t, t->size, Parser_sentence(parser));
		}
	}
	else if (parser->blockstyle == BEGINEND)
	{
		while (parser->lexer->token != TK_EOF && parser->lexer->token != TK_END)
		{
			ListObject_InsertItem(t, t->size, Parser_sentence(parser));
		}
	}
	else if (parser->blockstyle == END)
	{
		while (parser->lexer->token != TK_EOF && parser->lexer->token != TK_END)
		{
			ListObject_InsertItem(t, t->size, Parser_sentence(parser));
		}
	}
	else if (parser->blockstyle == ENDNAME)
	{
		while (parser->lexer->token != TK_EOF)
		{
			if (parser->lexer->token == TK_END)
			{
				Parser_match(parser, TK_END);
				if (parser->lexer->token == token)
				{
					Parser_match(parser, parser->lexer->token);
					break;
				}
				Parser_match(parser, parser->lexer->token);
				if (parser->lexer->token == TK_EOF)
				{
					break;
				}
			}
			ListObject_InsertItem(t, t->size, Parser_sentence(parser));
		}
	}
	return (struct Object*)t;
}

struct Object* Parser_sentence(struct Parser* parser)
{
	struct Object* t = NULL;
	switch (parser->lexer->token)
	{
	case TK_IF:
		t = Parser_if(parser);
		break;
	case TK_WHILE:
		t = Parser_while(parser);
		break;
	case TK_LLARGE:
		if (parser->blockstyle == BRACES)
		{
			Parser_match(parser, TK_LLARGE);
			t = Parser_body(parser, NULL,-1);
			Parser_match(parser, TK_RLARGE);
		}
		break;
	case TK_BEGIN:
		if (parser->blockstyle == BEGINEND)
		{
			Parser_match(parser, TK_BEGIN);
			t = Parser_body(parser, NULL,-1);
			Parser_match(parser, TK_END);
		}
		break;
	case TK_NUM:
	case TK_STRING:
	case TK_NAME:
	case TK_DOUBLE:
		t = Parser_exp(parser);
		break;
	case TK_DEF:
		t = Parser_functiondef(parser);
		break;
	case TK_BREAK:
	case TK_RETURN:
	case TK_CONTINUE:
		t = Parser_jump(parser);
		break;
	case TK_COMMENT:
		Parser_preprocess(parser);
		t = Parser_sentence(parser);
		break;
	case TK_IMPORT:
		t = Parser_import(parser);
		break;
	case TK_CLASS:
		t = Parser_class(parser);
		break;
	case TK_TRY:
		t = Parser_try(parser);
		break;
	default:
		printf("%s不能作为语句", parser->lexer->tokenstr->string);
		exit(-1);
	}
	if (t!=NULL&&CHECK(t, "Call"))
	{
		((struct CallASTObject*)t)->mode = StringObject_NewWithString("sentence");
	}
	return t;
}

void Parser_match(struct Parser* parser, int expect)
{
	if (parser->lexer->token != expect)
	{
		printf("不匹配的token:%s,应该为:%d", parser->lexer->tokenstr->string,expect);
		exit(-1);
	}
	Lexer_gettoken(parser->lexer);
}

struct Object* Parser_if(struct Parser* parser)
{
	struct IfASTObject* t = IfASTObject_NewWithParser(parser);
	Parser_match(parser, TK_IF);
	t->exp = Parser_exp_logical_or(parser);
	//TODO 不同的代码样式
	if (parser->blockstyle == BRACES)
	{
		struct Object* body = Parser_sentence(parser);
		if (!CHECK(body, "list"))
		{
			struct ListObject* p = ListObject_New();
			ListObject_InsertItem(p, 0, body);
			body = p;
		}
		t->body = body;
		if (parser->lexer->token == TK_ELSE)
		{
			Parser_match(parser, TK_ELSE);
			body= Parser_sentence(parser);
			if (!CHECK(body, "list"))
			{
				struct ListObject* p = ListObject_New();
				ListObject_InsertItem(p, 0, body);
				body = p;
			}
			t->elses = body;
		}
	}
	else if (parser->blockstyle == INDENT)
	{
		char* indent = parser->lexer->indent->string;
		Parser_match(parser, TK_COLON);
		t->body = Parser_body(parser, parser->lexer->indent->string,-1);
		if (!strcmp(indent,parser->lexer->indent->string)&&parser->lexer->token == TK_ELSE)
		{
			Parser_match(parser, TK_ELSE);
			Parser_match(parser, TK_COLON);
			t->elses = Parser_body(parser, parser->lexer->indent->string,-1);
		}
	}
	else if (parser->blockstyle == BEGINEND)
	{
		struct Object* body = Parser_sentence(parser);
		if (!CHECK(body, "list"))
		{
			struct ListObject* p = ListObject_New();
			ListObject_InsertItem(p, 0, body);
			body = p;
		}
		t->body = body;
		if (parser->lexer->token == TK_ELSE)
		{
			Parser_match(parser, TK_ELSE);
			body = Parser_sentence(parser);
			if (!CHECK(body, "list"))
			{
				struct ListObject* p = ListObject_New();
				ListObject_InsertItem(p, 0, body);
				body = p;
			}
			t->elses = body;
		}
	}
	else if (parser->blockstyle == END)
	{
		struct Object* body = Parser_body(parser,"",-1);
		t->body = body;
		if (parser->lexer->token == TK_ELSE)
		{
			Parser_match(parser, TK_ELSE);
			t->elses = Parser_body(parser, "", -1);
		}
	}
	else if (parser->blockstyle == ENDNAME)
	{
		struct Object* body = Parser_body(parser, "", TK_IF);
		t->body = body;
		if (parser->lexer->token == TK_ELSE)
		{
			Parser_match(parser, TK_ELSE);
			t->elses = Parser_body(parser, "", TK_ELSE);
		}
	}
	return (struct Object*)t;
}

struct Object* Parser_exp_logical_or(struct Parser* parser)
{
	struct Object* t = Parser_exp_logical_and(parser);
	if (parser->lexer->token == TK_OR)
	{
		struct OperationASTObject* p = OperationASTObject_NewWithParser(parser);
		p->op = parser->lexer->tokenstr;
		p->left = t;
		Parser_match(parser, TK_OR);
		p->right = Parser_exp_logical_and(parser);
		t = p;
	}
	return t;
}

struct Object* Parser_exp_logical_and(struct Parser* parser)
{
	struct Object* t = Parser_exp_equality(parser);
	if (parser->lexer->token == TK_AND)
	{
		struct OperationASTObject* p = OperationASTObject_NewWithParser(parser);
		p->op = parser->lexer->tokenstr;
		p->left = t;
		Parser_match(parser, TK_AND);
		p->right = Parser_exp_equality(parser);
		t = p;
	}
	return t;
}

struct Object* Parser_exp_equality(struct Parser* parser)
{
	struct Object* t = Parser_exp_additive(parser);
	if (parser->lexer->token == TK_EQ|| parser->lexer->token == TK_NEQ||
		parser->lexer->token == TK_GT|| parser->lexer->token == TK_GEQ||
		parser->lexer->token == TK_LT|| parser->lexer->token == TK_LEQ)
	{
		struct OperationASTObject* p = OperationASTObject_NewWithParser(parser);
		p->op = parser->lexer->tokenstr;
		p->left = t;
		Parser_match(parser, parser->lexer->token);
		p->right = Parser_exp_additive(parser);
		t = p;
	}
	return t;
}

struct Object* Parser_exp_additive(struct Parser* parser)
{
	struct Object* t = Parser_exp_multiplicative(parser);
	while (parser->lexer->token == TK_ADD|| parser->lexer->token == TK_SUB)
	{
		struct OperationASTObject* p = OperationASTObject_NewWithParser(parser);
		p->op = parser->lexer->tokenstr;
		p->left = t;
		Parser_match(parser, parser->lexer->token);
		p->right = Parser_exp_multiplicative(parser);
		t = p;
	}
	return t;
}
struct Object* Parser_exp_multiplicative(struct Parser* parser)
{
	struct Object* t = Parser_exp_postfix(parser);
	while (parser->lexer->token == TK_MUL || parser->lexer->token == TK_DIV||parser->lexer->token==TK_MOD)
	{
		struct OperationASTObject* p = OperationASTObject_NewWithParser(parser);
		p->op = parser->lexer->tokenstr;
		p->left = t;
		Parser_match(parser, parser->lexer->token);
		p->right = Parser_exp_postfix(parser);
		t = p;
	}
	return t;
}

struct Object* Parser_exp_postfix(struct Parser* parser)
{
	struct Object* t = Parser_exp_primary(parser);
	while (parser->lexer->token == TK_LMIDDLE||
			parser->lexer->token==TK_LLITTLE||
			parser->lexer->token==TK_POINT)
	{
		if (parser->lexer->token == TK_LMIDDLE)
		{
			struct SubscriptASTObject* p = SubscriptASTObject_NewWithParser(parser);
			p->value = t;
			Parser_match(parser, TK_LMIDDLE);
			p->slice = Parser_exp(parser);
			Parser_match(parser, TK_RMIDDLE);
			p->mode = StringObject_NewWithString("load");
			t = p;
		}
		else if (parser->lexer->token == TK_LLITTLE)
		{
			struct CallASTObject* p = CallASTObject_NewWithParser(parser);
			p->func = t;
			Parser_match(parser, TK_LLITTLE);
			if (parser->lexer->token != TK_RLITTLE)
			{
				p->args = Parser_exp_argument(parser);
			}
			Parser_match(parser, TK_RLITTLE);
			p->mode = StringObject_NewWithString("exp");
			t = p;
		}
		else if (parser->lexer->token == TK_POINT)
		{
			char* tokenstr = parser->lexer->tokenstr->string;
			struct AttributeASTObject* p = AttributeASTObject_NewWithParser(parser);
			p->value = t;
			Parser_match(parser, TK_POINT);
			p->attr = parser->lexer->tokenstr;
			Parser_match(parser, parser->lexer->token);
			/*两种方法获取命名空间里的变量*/
			if (strcmp(tokenstr, "::"))
			{
				p->mode = StringObject_NewWithString("load");
			}
			else
			{
				p->mode= StringObject_NewWithString("withoutself");
			}

			struct Object* result=NULL;
			if (CHECK(t, "Name"))
			{
				result = DictObject_GetItem(parser->namespace_names, ((struct NameASTObject*)t)->id);
			}
			else if (CHECK(t, "Attribute"))
			{
				result = DictObject_GetItem(parser->namespace_names, ((struct AttributeASTObject*)t)->attr);
			}
			if (result != NULL)
			{
				p->mode = StringObject_NewWithString("withoutself");
			}

			t = p;
		}
	}
	return t;
}

struct Object* Parser_exp_primary(struct Parser* parser)
{
	struct Object* t = NULL;
	if (parser->lexer->token == TK_NAME)
	{
		struct NameASTObject* p = NameASTObject_NewWithParser(parser);
		p->id = parser->lexer->tokenstr;
		if (parser->importmode)
		{
			p->mode= StringObject_NewWithString("import");
		}
		else
		{
			p->mode = StringObject_NewWithString("load");
		}
		Parser_match(parser, parser->lexer->token);
		t = p;
	}
	else if (parser->lexer->token == TK_NUM)
	{
		struct ConstantASTObject* p = ConstantASTObject_NewWithParser(parser);
		p->value = IntObject_NewWithValue(atoi(parser->lexer->tokenstr->string));
		Parser_match(parser, parser->lexer->token);
		t = p;
	}
	else if (parser->lexer->token == TK_STRING)
	{
		struct ConstantASTObject* p = ConstantASTObject_NewWithParser(parser);
		p->value = parser->lexer->tokenstr;
		Parser_match(parser, parser->lexer->token);
		t = p;
	}
	else if (parser->lexer->token == TK_DOUBLE)
	{
		struct ConstantASTObject* p = ConstantASTObject_NewWithParser(parser);
		p->value = DoubleObject_NewWithValue(atof(parser->lexer->tokenstr->string));
		Parser_match(parser, parser->lexer->token);
		t = p;
	}
	else if (parser->lexer->token == TK_LLITTLE)
	{
		Parser_match(parser, TK_LLITTLE);
		if (parser->lexer->token != TK_RLITTLE)
		{
			t = Parser_exp(parser);
		}
		Parser_match(parser, TK_RLITTLE);
	}
	else if (parser->lexer->token == TK_LMIDDLE)
	{
		struct ListASTObject* p = ListASTObject_NewWithParser(parser);
		Parser_match(parser, TK_LMIDDLE);
		if (parser->lexer->token != TK_RMIDDLE)
		{
			ListObject_InsertItem(p->list, p->list->size, Parser_exp(parser));
			while (parser->lexer->token == TK_COMMA)
			{
				Parser_match(parser, TK_COMMA);
				ListObject_InsertItem(p->list, p->list->size, Parser_exp(parser));
			}
		}
		Parser_match(parser, TK_RMIDDLE);
		t = p;
	}
	return t;
}

struct Object* Parser_exp(struct Parser* parser)
{
	struct Object* t = Parser_assignment(parser);
	return t;
}

struct Object* Parser_assignment(struct Parser* parser)
{
	struct Object* t = Parser_exp_logical_or(parser);
	if (parser->lexer->token == TK_ASSIGN)
	{
		struct AssignASTObject* p = AssignASTObject_NewWithParser(parser);
		if (CHECK(t, "Name"))
		{
			((struct NameASTObject*)t)->mode = StringObject_NewWithString("store");
		}
		else if (CHECK(t, "Subscript"))
		{
			((struct SubscriptASTObject*)t)->mode = StringObject_NewWithString("store");
		}
		else if (CHECK(t, "Attribute"))
		{
			((struct AttributeASTObject*)t)->mode = StringObject_NewWithString("store");
		}
		ListObject_InsertItem(p->targets, p->targets->size, t);
		Parser_match(parser, TK_ASSIGN);
		p->value = Parser_exp_logical_or(parser);
		t = p;
	}
	return t;
}

struct Object* Parser_exp_argument(struct Parser* parser)
{
	struct ListObject* t = ListObject_New();
	struct Object* p = Parser_exp(parser);
	ListObject_InsertItem(t, t->size, p);
	while (parser->lexer->token == TK_COMMA)
	{
		Parser_match(parser, TK_COMMA);
		struct Object* p = Parser_exp(parser);
		ListObject_InsertItem(t, t->size, p);
	}
	return t;
}

struct Object* Parser_jump(struct Parser* parser)
{
	struct Object* t = NULL;
	if (parser->lexer->token == TK_RETURN)
	{
		Parser_match(parser, TK_RETURN);
		struct ReturnASTObject* p = ReturnASTObject_NewWithParser(parser);
		p->value = Parser_exp(parser);
		t = p;
	}
	else if (parser->lexer->token == TK_BREAK)
	{
		struct BreakASTObject* p = BreakASTObject_NewWithParser(parser);
		Parser_match(parser, TK_BREAK);
		t = p;
	}
	else if (parser->lexer->token == TK_CONTINUE)
	{
		struct ContinueASTObject* p = ContinueASTObject_NewWithParser(parser);
		Parser_match(parser, TK_CONTINUE);
		t = p;
	}
	return t;
}

struct Object* Parser_functiondef(struct Parser* parser)
{
	struct FunctionDefASTObject* t = FunctionDefASTObject_NewWithParser(parser);
	Parser_match(parser, TK_DEF);
	t->name = parser->lexer->tokenstr;
	Parser_match(parser, parser->lexer->token);
	Parser_match(parser, TK_LLITTLE);
	if (parser->lexer->token != TK_RLITTLE)
	{
		struct ListObject* t2 = ListObject_New();
		struct NameASTObject* p = Parser_exp_primary(parser);
		p->mode = StringObject_NewWithString("arg");
		ListObject_InsertItem(t2, t2->size, p);
		while (parser->lexer->token == TK_COMMA)
		{
			Parser_match(parser, TK_COMMA);
			struct NameASTObject* p = Parser_exp_primary(parser);
			p->mode = StringObject_NewWithString("arg");
			ListObject_InsertItem(t2, t2->size, p);
		}
		t->args = t2;
	}
	Parser_match(parser, TK_RLITTLE);
	//TODO 不同的代码样式
	if (parser->blockstyle == BRACES)
	{
		Parser_match(parser, TK_LLARGE);
		t->body = Parser_body(parser, "",-1);
		Parser_match(parser, TK_RLARGE);
	}
	else if (parser->blockstyle == INDENT)
	{
		Parser_match(parser, TK_COLON);
		t->body = Parser_body(parser, parser->lexer->indent->string,-1);
	}
	else if (parser->blockstyle == BEGINEND)
	{
		Parser_match(parser, TK_BEGIN);
		t->body = Parser_body(parser, "", -1);
		Parser_match(parser, TK_END);
	}
	else if (parser->blockstyle == END)
	{
		t->body = Parser_body(parser, "", -1);
		Parser_match(parser, TK_END);
	}
	else if (parser->blockstyle == ENDNAME)
	{
		t->body = Parser_body(parser, "", TK_DEF);
	}
	return t;
}

struct Object* Parser_while(struct Parser* parser)
{
	struct WhileASTObject* t = WhileASTObject_NewWithParser(parser);
	Parser_match(parser, TK_WHILE);
	t->exp = Parser_exp(parser);
	if (parser->blockstyle == BRACES)
	{
		struct Object* body = Parser_sentence(parser);
		if (!CHECK(body, "list"))
		{
			struct ListObject* p = ListObject_New();
			ListObject_InsertItem(p, 0, body);
			body = p;
		}
		t->body = body;
	}
	else if (parser->blockstyle == INDENT)
	{
		Parser_match(parser, TK_COLON);
		t->body = Parser_body(parser, parser->lexer->indent->string, -1);
	}
	else if (parser->blockstyle == BEGINEND)
	{
		struct Object* body = Parser_sentence(parser);
		if (!CHECK(body, "list"))
		{
			struct ListObject* p = ListObject_New();
			ListObject_InsertItem(p, 0, body);
			body = p;
		}
		t->body = body;
	}
	else if (parser->blockstyle == END)
	{
		struct Object* body = Parser_body(parser, "", -1);
		t->body = body;
	}
	else if (parser->blockstyle == ENDNAME)
	{
		struct Object* body = Parser_body(parser, "", TK_WHILE);
		t->body = body;
	}
	return t;
}

void Parser_preprocess(struct Parser* parser)
{
	Parser_match(parser, TK_COMMENT);
	if (!strcmp(parser->lexer->tokenstr->string, "blockstyle"))
	{
		Parser_match(parser, parser->lexer->token);
		if (!strcmp(parser->lexer->tokenstr->string, "indent"))
		{
			parser->blockstyle = INDENT;
		}
		else if (!strcmp(parser->lexer->tokenstr->string, "braces"))
		{
			parser->blockstyle = BRACES;
		}
		else if (!strcmp(parser->lexer->tokenstr->string, "end"))
		{
			parser->blockstyle = END;
		}
		else if (!strcmp(parser->lexer->tokenstr->string, "endname"))
		{
			parser->blockstyle = ENDNAME;
		}
		else if (!strcmp(parser->lexer->tokenstr->string, "beginend"))
		{
			parser->blockstyle = BEGINEND;
		}
	}
}

struct Object* Parser_import(struct Parser* parser)
{
	struct ImportASTObject* t = ImportASTObject_NewWithParser(parser);
	Parser_match(parser,TK_IMPORT);
	parser->importmode = 1;
	t->name = Parser_exp_postfix(parser);
	parser->importmode = 0;
	if (CHECK(t->name, "Name"))
	{
		DictObject_SetItem(parser->namespace_names, ((struct NameASTObject*)t->name)->id, IntObject_New());
	}
	return t;
}

struct Object* Parser_class(struct Parser* parser)
{
	struct ClassASTObject* t = ClassASTObject_NewWithParser(parser);
	Parser_match(parser, TK_CLASS);

	t->name = parser->lexer->tokenstr;
	Parser_match(parser, parser->lexer->token);

	if (parser->lexer->token == TK_LLITTLE)
	{
		Parser_match(parser,TK_LLITTLE);
		struct Object* p = Parser_exp(parser);
		ListObject_InsertItem(t->bases, t->bases->size, p);
		while (parser->lexer->token == TK_COMMA)
		{
			Parser_match(parser, TK_COMMA);
			struct Object* p = Parser_exp(parser);
			ListObject_InsertItem(t->bases, t->bases->size, p);
		}
		Parser_match(parser, TK_RLITTLE);
	}

	if (parser->blockstyle == BRACES)
	{
		Parser_match(parser, TK_LLARGE);
		if (parser->lexer->token != TK_RLARGE)
		{
			t->body = Parser_body(parser,"",-1);
		}
		Parser_match(parser, TK_RLARGE);
	}
	else if (parser->blockstyle == INDENT)
	{
		Parser_match(parser, TK_COLON);
		t->body = Parser_body(parser, parser->lexer->indent->string, -1);
	}
	else if (parser->blockstyle == BEGINEND)
	{
		Parser_match(parser, TK_BEGIN);
		if (parser->lexer->token != TK_RLARGE)
		{
			t->body = Parser_body(parser, "", -1);
		}
		Parser_match(parser, TK_END);
	}
	else if (parser->blockstyle == END)
	{
		struct Object* body = Parser_body(parser, "", -1);
		t->body = body;
	}
	else if (parser->blockstyle == ENDNAME)
	{
		struct Object* body = Parser_body(parser, "", TK_CLASS);
		t->body = body;
	}
	return t;
}

struct Object* Parser_try(struct Parser* parser)
{
	struct TryASTObject* t = TryASTObject_NewWithParser(parser);
	Parser_match(parser, TK_TRY);

	if (parser->blockstyle == BRACES)
	{
		Parser_match(parser, TK_LLARGE);
		if (parser->lexer->token != TK_RLARGE)
		{
			t->body = Parser_body(parser, "", -1);
		}
		Parser_match(parser, TK_RLARGE);
	}
	else if (parser->blockstyle == INDENT)
	{
		Parser_match(parser, TK_COLON);
		t->body = Parser_body(parser, parser->lexer->indent->string, -1);
	}
	else if (parser->blockstyle == BEGINEND)
	{
		Parser_match(parser, TK_BEGIN);
		if (parser->lexer->token != TK_RLARGE)
		{
			t->body = Parser_body(parser, "", -1);
		}
		Parser_match(parser, TK_END);
	}
	else if (parser->blockstyle == END)
	{
		struct Object* body = Parser_body(parser, "", -1);
		t->body = body;
	}
	else if (parser->blockstyle == ENDNAME)
	{
		struct Object* body = Parser_body(parser, "", TK_CLASS);
		t->body = body;
	}

	Parser_match(parser, TK_EXCEPT);

	if (parser->blockstyle == BRACES)
	{
		Parser_match(parser, TK_LLARGE);
		if (parser->lexer->token != TK_RLARGE)
		{
			t->handle = Parser_body(parser, "", -1);
		}
		Parser_match(parser, TK_RLARGE);
	}
	else if (parser->blockstyle == INDENT)
	{
		Parser_match(parser, TK_COLON);
		t->handle = Parser_body(parser, parser->lexer->indent->string, -1);
	}
	else if (parser->blockstyle == BEGINEND)
	{
		Parser_match(parser, TK_BEGIN);
		if (parser->lexer->token != TK_RLARGE)
		{
			t->handle = Parser_body(parser, "", -1);
		}
		Parser_match(parser, TK_END);
	}
	else if (parser->blockstyle == END)
	{
		struct Object* body = Parser_body(parser, "", -1);
		t->handle = body;
	}
	else if (parser->blockstyle == ENDNAME)
	{
		struct Object* body = Parser_body(parser, "", TK_CLASS);
		t->handle = body;
	}
	return t;
}