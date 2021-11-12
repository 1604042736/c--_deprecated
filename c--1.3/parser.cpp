#include "Include/parser.h"

SyntaxTree* syntaxtree_init(NodeType nt, ASTType at, int ln, int lp)
{
	SyntaxTree* syntaxtree = new SyntaxTree;
	syntaxtree->nodetype = nt;
	syntaxtree->asttype = at;
	syntaxtree->lineno = ln;
	syntaxtree->linepos = lp;
	return syntaxtree;
}

void syntaxtree_print(SyntaxTree* syntaxtree, int space)
{
	if (syntaxtree == NULL)return;
	if (syntaxtree->asttype == AT_SYNTAXTREE)
	{
		for (int i = 0; i < space; i++)cout << "  ";
		switch (syntaxtree->nodetype)
		{
		case NT_NAME:
			cout << "Name" << endl;
			break;
		case NT_PROGRAM:
			cout << "Program" << endl;
			break;
		case NT_CONDITION:
			cout << "Condition" << endl;
			break;
		case NT_OPERATION:
			cout << "Operation" << endl;
			break;
		case NT_LOOP:
			cout << "Loop" << endl;
			break;
		case NT_ASSIGN:
			cout << "Assign" << endl;
			break;
		case NT_FUNCTIONDEF:
			cout << "FunctionDef" << endl;
			break;
		case NT_SUBSCRIPT:
			cout << "Subscript" << endl;
			break;
		case NT_CALL:
			cout << "Call" << endl;
			break;
		case NT_BREAK:
			cout << "Break" << endl;
			break;
		case NT_CONTINUE:
			cout << "Continue" << endl;
			break;
		case NT_RETURN:
			cout << "Return" << endl;
			break;
		case NT_NUM:
			cout << "Num" << endl;
			break;
		case NT_STRING:
			cout << "String" << endl;
			break;
		case NT_LIST:
			cout << "List" << endl;
			break;
		case NT_ATTRIBUTE:
			cout << "Attribute" << endl;
			break;
		}
		map<string, SyntaxTree*>::iterator iter;
		iter = syntaxtree->child.begin();
		while (iter != syntaxtree->child.end())
		{
			for (int i = 0; i < space + 1; i++)cout << "  ";
			cout << iter->first;
			if (iter->second != NULL)
			{
				if (iter->second->asttype == AT_SYNTAXTREE)
				{
					cout << endl;
					syntaxtree_print(iter->second, space + 2);
				}
				else if (iter->second->asttype == AT_LIST)
				{
					cout << endl;
					syntaxtree_print(iter->second, space + 1);
				}
				else if (iter->second->asttype == AT_STRING)
				{
					cout << ": " << iter->second->str << endl;
				}
				else if (iter->second->asttype == AT_INT)
				{
					cout <<": " << iter->second->value << endl;
				}
			}
			else
			{
				cout << endl;
			}
			iter++;
		}
	}
	else if(syntaxtree->asttype==AT_LIST)
	{
		for (int i = 0; i < syntaxtree->list.size(); i++)syntaxtree_print(syntaxtree->list[i], space + 1);
	}
}

Parser* parser_init(Lexer* lexer,BlockStyle blockstyle)
{
	Parser* parser = new Parser;
	parser->lexer = lexer;
	parser->blockstyle = blockstyle;
	parser->lexer->blockstyle = blockstyle;
	return parser;
}

SyntaxTree* parser_parse(Parser* parser)
{
	lexer_getch(parser->lexer);
	lexer_gettoken(parser->lexer);
	SyntaxTree* t = syntaxtree_init(NT_PROGRAM, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	t->child["body"] = parser_body(parser,"",TK_EOF);
	return t;
}

void parser_match(Parser* parser, enum Token expect)
{
	if (parser->lexer->token != expect)
	{
		cout << "意料之外的Token: " << parser->lexer->token << ",应该为: " << expect;
		exit(-1);
	}
	lexer_gettoken(parser->lexer);
}

SyntaxTree* parser_body(Parser* parser,string indent,Token endtoken)
{
	SyntaxTree* t = syntaxtree_init(NT_NULL, AT_LIST, parser->lexer->lineno, parser->lexer->linepos);
	if (parser->blockstyle == INDENT)
	{
		while (parser->lexer->token != TK_EOF && indent==parser->lexer->indent)
		{
			t->list.push_back(parser_sentence(parser));
		}
	}
	else if (parser->blockstyle == BEGINEND)
	{
		while (parser->lexer->token != TK_EOF && parser->lexer->token != TK_END)
		{
			t->list.push_back(parser_sentence(parser));
		}
	}
	else if (parser->blockstyle == BRACES)
	{
		while (parser->lexer->token != TK_EOF && parser->lexer->token != TK_RLARGE)
		{
			t->list.push_back(parser_sentence(parser));
		}
	}
	else if (parser->blockstyle == END)
	{
		while (parser->lexer->token != TK_EOF)
		{
			t->list.push_back(parser_sentence(parser));
			if (parser->lexer->token == TK_END)
			{
				parser_match(parser,TK_END);
				if (parser->lexer->token == endtoken)
				{
					parser_match(parser, endtoken);
					break;
				}
			}
		}
	}
	return t;
}

SyntaxTree* parser_sentence(Parser* parser)
{
	SyntaxTree* t = NULL;
	switch (parser->lexer->token)
	{
	case TK_IF:
		t = parser_if(parser);
		break;
	case TK_NAME:
		t = parser_assign(parser);
		break;
	case TK_WHILE:
		t=parser_while(parser);
		break;
	case TK_SEMI:
		parser_match(parser, TK_SEMI);
		t = parser_sentence(parser);
		break;
	case TK_DEF:
		t = parser_def(parser);
		break;
	case TK_BREAK:
		t = parser_break(parser);
		break;
	case TK_CONTINUE:
		t = parser_continue(parser);
		break;
	case TK_RETURN:
		t = parser_return(parser);
		break;
	default:
		cout << parser->lexer->token << "不能作为语句";
		exit(-1);
		break;
	}
	//TODO 不同的代码样式
	if (parser->blockstyle==BRACES&&parser->lexer->token == TK_LLARGE)
	{
		parser_match(parser, TK_LLARGE);
		t = parser_body(parser,"",TK_EOF);
		parser_match(parser, TK_RLARGE);
	}
	else if (parser->blockstyle == BEGINEND && parser->lexer->token == TK_BEGIN)
	{
		parser_match(parser, TK_BEGIN);
		t = parser_body(parser, "",TK_EOF);
		parser_match(parser, TK_END);
	}
	return t;
}

SyntaxTree* parser_if(Parser* parser)
{
	SyntaxTree* t = syntaxtree_init(NT_CONDITION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_IF);
	t->child["exp"] = parser_exp(parser);
	if (parser->blockstyle == INDENT)
	{
		parser_match(parser, TK_COLON);
		t->child["body"] = parser_body(parser, parser->lexer->indent,TK_EOF);
		t->child["elses"] = NULL;
		if (parser->lexer->token == TK_ELSE)
		{
			parser_match(parser, TK_ELSE);
			parser_match(parser, TK_COLON);
			t->child["elses"] = parser_body(parser, parser->lexer->indent,TK_EOF);
		}
	}
	else if (parser->blockstyle == BEGINEND)
	{
		t->child["body"] = parser_sentence(parser);
		t->child["elses"] = NULL;
		if (parser->lexer->token == TK_ELSE)
		{
			parser_match(parser, TK_ELSE);
			t->child["elses"] = parser_sentence(parser);
		}
	}
	else if (parser->blockstyle == BRACES)
	{
		t->child["body"] = parser_sentence(parser);
		t->child["elses"] = NULL;
		if (parser->lexer->token == TK_ELSE)
		{
			parser_match(parser, TK_ELSE);
			t->child["elses"] = parser_sentence(parser);
		}
	}
	else if (parser->blockstyle == END)
	{
		t->child["body"] = parser_body(parser,"",TK_IF);
		t->child["elses"] = NULL;
		if (parser->lexer->token == TK_ELSE)
		{
			parser_match(parser, TK_ELSE);
			t->child["elses"] = parser_body(parser, "", TK_ELSE);
		}
	}
	return t;
}

SyntaxTree* parser_exp(Parser* parser)
{
	return parser_exp_or(parser);
}

SyntaxTree* parser_exp_or(Parser* parser)
{
	struct SyntaxTree* t = parser_exp_and(parser);
	while (parser->lexer->token == TK_OR)
	{
		parser_match(parser, TK_OR);
		struct SyntaxTree* p = syntaxtree_init(NT_OPERATION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"]= syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"]->str = "or";
		p->child["left"] = t;
		p->child["right"] = parser_exp_and(parser);
		t = p;
	}
	return t;
}

SyntaxTree* parser_exp_and(Parser* parser)
{
	struct SyntaxTree* t = parser_exp_compare(parser);
	while (parser->lexer->token == TK_AND)
	{
		parser_match(parser, TK_AND);
		struct SyntaxTree* p = syntaxtree_init(NT_OPERATION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"]->str = "and";
		p->child["left"] = t;
		p->child["right"] = parser_exp_compare(parser);
		t = p;
	}
	return t;
}

SyntaxTree* parser_exp_compare(Parser* parser)
{
	SyntaxTree* t = parser_exp_shift(parser);
	if (parser->lexer->token == TK_GT || parser->lexer->token == TK_GEQ || parser->lexer->token == TK_LT ||
		parser->lexer->token == TK_LEQ || parser->lexer->token == TK_EQ || parser->lexer->token == TK_NEQ)
	{
		string op = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
		struct SyntaxTree* p = syntaxtree_init(NT_OPERATION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"]->str = op;
		p->child["left"] = t;
		p->child["right"] = parser_exp_shift(parser);
		t = p;
	}
	return t;
}

SyntaxTree* parser_exp_shift(Parser* parser)
{
	SyntaxTree* t = parser_exp_add(parser);
	while (parser->lexer->token == TK_LSHIFT || parser->lexer->token == TK_RSHIFT)
	{
		string op = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
		struct SyntaxTree* p = syntaxtree_init(NT_OPERATION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"]->str = op;
		p->child["left"] = t;
		p->child["right"] = parser_exp_add(parser);
		t = p;
	}
	return t;
}

SyntaxTree* parser_exp_add(Parser* parser)
{
	SyntaxTree* t = parser_exp_mul(parser);
	while (parser->lexer->token == TK_ADD || parser->lexer->token == TK_SUB)
	{
		string op = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
		struct SyntaxTree* p = syntaxtree_init(NT_OPERATION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"]->str = op;
		p->child["left"] = t;
		p->child["right"] = parser_exp_mul(parser);
		t = p;
	}
	return t;
}

SyntaxTree* parser_exp_mul(Parser* parser)
{
	SyntaxTree* t = parser_exp_primary(parser);
	while (parser->lexer->token == TK_MUL || parser->lexer->token == TK_DIV || parser->lexer->token == TK_MOD)
	{
		string op = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
		struct SyntaxTree* p = syntaxtree_init(NT_OPERATION, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child["op"]->str = op;
		p->child["left"] = t;
		p->child["right"] = parser_exp_primary(parser);
		t = p;
	}
	return t;
}

SyntaxTree* parser_exp_primary(Parser* parser)
{
	SyntaxTree* t = NULL;
	if (parser->lexer->token == TK_NAME)
	{
		t = syntaxtree_init(NT_NAME, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		t->child["id"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		t->child["id"]->str = parser->lexer->tokenstr;
		t->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		t->child["mode"]->str = "load";
		parser_match(parser, parser->lexer->token);

		while (parser->lexer->token == TK_POINT)
		{
			parser_match(parser, parser->lexer->token);
			SyntaxTree* p = syntaxtree_init(NT_ATTRIBUTE, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			p->child["value"] = t;
			p->child["attr"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
			p->child["attr"]->str = parser->lexer->tokenstr;
			p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
			p->child["mode"]->str = "load";
			parser_match(parser, parser->lexer->token);
			t = p;

			if (parser->lexer->token == TK_LMIDDLE)
			{
				while (parser->lexer->token == TK_LMIDDLE)
				{
					parser_match(parser, TK_LMIDDLE);
					SyntaxTree* p = syntaxtree_init(NT_SUBSCRIPT, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
					p->child["value"] = t;
					p->child["slice"] = parser_exp(parser);
					p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
					p->child["mode"]->str = "load";
					t = p;
					parser_match(parser, TK_RMIDDLE);
				}
			}
			if (parser->lexer->token == TK_LLITTLE)
			{
				while (parser->lexer->token == TK_LLITTLE)
				{
					SyntaxTree* p = syntaxtree_init(NT_CALL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
					p->child["func"] = t;
					p->child["args"] = parser_args(parser);
					p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
					p->child["mode"]->str = "exp";
					t = p;
				}
			}
		}

		if (parser->lexer->token == TK_LMIDDLE)
		{
			while (parser->lexer->token == TK_LMIDDLE)
			{
				parser_match(parser, TK_LMIDDLE);
				SyntaxTree* p = syntaxtree_init(NT_SUBSCRIPT, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
				p->child["value"] = t;
				p->child["slice"] = parser_exp(parser);
				p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
				p->child["mode"]->str = "load";
				t = p;
				parser_match(parser, TK_RMIDDLE);
			}
		}
		if (parser->lexer->token == TK_LLITTLE)
		{
			while (parser->lexer->token == TK_LLITTLE)
			{
				SyntaxTree* p = syntaxtree_init(NT_CALL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
				p->child["func"] = t;
				p->child["args"] = parser_args(parser);
				p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
				p->child["mode"]->str = "exp";
				t = p;
			}
		}
	}
	else if (parser->lexer->token == TK_NUM)
	{
		t = syntaxtree_init(NT_NUM, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		t->child["value"] = syntaxtree_init(NT_NULL, AT_INT, parser->lexer->lineno, parser->lexer->linepos);
		t->child["value"]->value = atoi(parser->lexer->tokenstr.data());
		parser_match(parser, parser->lexer->token);
	}
	else if (parser->lexer->token == TK_STRING)
	{
		t = syntaxtree_init(NT_STRING, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		t->child["value"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		t->child["value"]->str = parser->lexer->tokenstr;
		parser_match(parser, parser->lexer->token);
	}
	else if (parser->lexer->token == TK_LLITTLE)
	{
		parser_match(parser, TK_LLITTLE);
		if (parser->lexer->token != TK_RLITTLE)
		{
			t = parser_exp(parser);
		}
		parser_match(parser, TK_RLITTLE);
	}
	else if (parser->lexer->token == TK_LMIDDLE)
	{
		t = syntaxtree_init(NT_NULL, AT_LIST, parser->lexer->lineno, parser->lexer->linepos);
		parser_match(parser, TK_LMIDDLE);
		if (parser->lexer->token != TK_RMIDDLE)
		{
			t->list.push_back(parser_exp(parser));
			while (parser->lexer->token == TK_COMMA)
			{
				parser_match(parser,parser->lexer->token);
				t->list.push_back(parser_exp(parser));
			}
		}
		parser_match(parser, TK_RMIDDLE);
		SyntaxTree* p= syntaxtree_init(NT_LIST, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["list"] = t;
		t = p;
	}
	return t;
}

SyntaxTree* parser_while(Parser* parser)
{
	SyntaxTree* t = syntaxtree_init(NT_LOOP, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_WHILE);
	t->child["exp"] = parser_exp(parser);
	if (parser->blockstyle == INDENT)
	{
		parser_match(parser, TK_COLON);
		t->child["body"] = parser_body(parser,parser->lexer->indent,TK_EOF);
	}
	else if (parser->blockstyle == BEGINEND)
	{
		t->child["body"] = parser_sentence(parser);
	}
	else if (parser->blockstyle == BRACES)
	{
		t->child["body"] = parser_sentence(parser);
	}
	else if (parser->blockstyle == END)
	{
		t->child["body"] = parser_body(parser, "", TK_WHILE);
	}
	return t;
}

SyntaxTree* parser_assign(Parser* parser)
{
	SyntaxTree* t = syntaxtree_init(NT_NAME, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	t->child["id"]= syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
	t->child["id"]->str = parser->lexer->tokenstr;
	t->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
	t->child["mode"]->str = "load";
	parser_match(parser, TK_NAME);

	while (parser->lexer->token == TK_POINT)
	{
		parser_match(parser, parser->lexer->token);
		SyntaxTree* p= syntaxtree_init(NT_ATTRIBUTE, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["value"] = t;
		p->child["attr"]= syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child["attr"]->str = parser->lexer->tokenstr;
		p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
		p->child["mode"]->str = "load";
		parser_match(parser, parser->lexer->token);
		t = p;

		if (parser->lexer->token == TK_LMIDDLE)
		{
			while (parser->lexer->token == TK_LMIDDLE)
			{
				parser_match(parser, TK_LMIDDLE);
				SyntaxTree* p = syntaxtree_init(NT_SUBSCRIPT, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
				p->child["value"] = t;
				p->child["slice"] = parser_exp(parser);
				p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
				p->child["mode"]->str = "load";
				t = p;
				parser_match(parser, TK_RMIDDLE);
			}
		}
		if (parser->lexer->token == TK_LLITTLE)
		{
			while (parser->lexer->token == TK_LLITTLE)
			{
				SyntaxTree* p = syntaxtree_init(NT_CALL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
				p->child["func"] = t;
				p->child["args"] = parser_args(parser);
				p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
				p->child["mode"]->str = "sentence";
				t = p;
			}
		}
	}

	if (parser->lexer->token==TK_LMIDDLE)
	{
		while (parser->lexer->token == TK_LMIDDLE)
		{
			parser_match(parser, TK_LMIDDLE);
			SyntaxTree* p= syntaxtree_init(NT_SUBSCRIPT, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			p->child["value"] = t;
			p->child["slice"] = parser_exp(parser);
			p->child["mode"] = syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
			p->child["mode"]->str = "load";
			t = p;
			parser_match(parser, TK_RMIDDLE);
		}
	}
	if (parser->lexer->token == TK_LLITTLE)
	{
		while (parser->lexer->token == TK_LLITTLE)
		{
			SyntaxTree* p = syntaxtree_init(NT_CALL, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
			p->child["func"] = t;
			p->child["args"] = parser_args(parser);
			p->child["mode"]= syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
			p->child["mode"]->str = "sentence";
			t = p;
		}
	}
	else if (parser->lexer->token == TK_ASSIGN)
	{
		t->child["mode"]->str = "store";
		SyntaxTree* p = syntaxtree_init(NT_ASSIGN, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
		p->child["targets"] = syntaxtree_init(NT_NULL, AT_LIST, parser->lexer->lineno, parser->lexer->linepos);
		p->child["targets"]->list.push_back(t);
		parser_match(parser, TK_ASSIGN);
		p->child["value"] = parser_exp(parser);
		t = p;
	}
	return t;
}

SyntaxTree* parser_def(Parser* parser)
{
	SyntaxTree* t = syntaxtree_init(NT_FUNCTIONDEF, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_DEF);
	t->child["name"]= syntaxtree_init(NT_NULL, AT_STRING, parser->lexer->lineno, parser->lexer->linepos);
	t->child["name"]->str = parser->lexer->tokenstr;
	parser_match(parser, TK_NAME);
	t->child["args"] = parser_args(parser);
	/*方便转成字节码*/
	for (int i = 0; i < t->child["args"]->list.size(); i++)
	{
		t->child["args"]->list[i]->nodetype = NT_STRING;
		t->child["args"]->list[i]->child["value"] = t->child["args"]->list[i]->child["id"];
	}
	if (parser->blockstyle == INDENT)
	{
		parser_match(parser, TK_COLON);
		t->child["body"] = parser_body(parser, parser->lexer->indent,TK_EOF);
	}
	else if (parser->blockstyle == BEGINEND)
	{
		parser_match(parser, TK_BEGIN);
		t->child["body"] = parser_body(parser, "",TK_EOF);
		parser_match(parser, TK_END);
	}
	else if (parser->blockstyle == BRACES)
	{
		parser_match(parser, TK_LLARGE);
		t->child["body"] = parser_body(parser, "",TK_EOF);
		parser_match(parser, TK_RLARGE);
	}
	else if (parser->blockstyle == END)
	{
		t->child["body"] = parser_body(parser, "", TK_DEF);
	}
	return t;
}

SyntaxTree* parser_args(Parser* parser)
{
	SyntaxTree*t= syntaxtree_init(NT_NULL, AT_LIST, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_LLITTLE);
	if (parser->lexer->token != TK_RLITTLE)
	{
		t->list.push_back(parser_exp(parser));
		while (parser->lexer->token == TK_COMMA)
		{
			parser_match(parser, TK_COMMA);
			SyntaxTree* p = parser_exp(parser);
			t->list.push_back(p);
		}
	}
	parser_match(parser, TK_RLITTLE);
	return t;
}

SyntaxTree* parser_break(Parser* parser)
{
	SyntaxTree* t= syntaxtree_init(NT_BREAK, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_BREAK);
	return t;
}

SyntaxTree* parser_continue(Parser* parser)
{
	SyntaxTree* t = syntaxtree_init(NT_CONTINUE, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_CONTINUE);
	return t;
}

SyntaxTree* parser_return(Parser* parser)
{
	SyntaxTree* t = syntaxtree_init(NT_RETURN, AT_SYNTAXTREE, parser->lexer->lineno, parser->lexer->linepos);
	parser_match(parser, TK_RETURN);
	t->child["value"] = parser_exp(parser);
	return t;
}