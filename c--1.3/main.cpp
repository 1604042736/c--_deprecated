#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "preprocessor.h"
#include "parser.h"
#include "globals.h"
#include "generator.h"
#include "eval.h"
using namespace std;

void printcodeobj(CodeObject* codeobj)
{
	vector<Object*> funccodeobj;

	cout << "Names: ";
	print(codeobj->names);
	cout << endl;
	cout << "Consts: ";

	ListObject* lobj = (ListObject*)codeobj->consts;
	cout << "[";
	for (int i = 0; i < lobj->list.size(); i++)
	{
		
		print(lobj->list[i]);
		if (i != lobj->list.size() - 1)
			cout << ",";
		if (lobj->list[i]->objattr->obj_name == "Code")
		{
			funccodeobj.push_back(lobj->list[i]);
		}
	}
	cout << "]";

	cout << endl;
	cout << "Code: \n";
	ListObject* codelist = (ListObject*)codeobj->code;
	int i = 0;
	while (i < codelist->list.size())
	{
		switch (((IntObject*)codelist->list[i])->val)
		{
		case LOAD_NAME:
		{
			cout << "LOAD_NAME ";

			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case LOAD_CONST:
		{
			cout << "LOAD_CONST ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case STORE_NAME:
		{
			cout << "STORE_NAME ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case ADD:
			cout << "ADD\n";
			break;
		case SUB:
			cout << "SUB\n";
			break;
		case MUL:
			cout << "MUL\n";
			break;
		case DIV:
			cout << "DIV\n";
			break;
		case MOD:
			cout << "MOD\n";
			break;
		case EQ:
			cout << "EQ\n";
			break;
		case IFJMP:
		{
			cout << "IFJMP ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case JMP:
		{
			cout << "JMP ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case RETURN:
			cout << "RETURN\n";
			break;
		case LOAD_VALUE:
		{
			cout << "LOAD_VALUE ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case MAKE_FUNCTION:
		{
			cout << "MAKE_FUNCTION ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case CALL:
		{
			cout << "CALL ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case OR:
			cout << "OR\n";
			break;
		case LOAD_SUBSCRIPT:
			cout << "LOAD_SUBSCRIPT\n";
			break;
		case STORE_SUBSCRIPT:
			cout << "STORE_SUBSCRIPT\n";
			break;
		case BUILD_LIST:
		{
			cout << "BUILD_LIST ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}

		case POP_TOP:
		{
			cout << "POP_TOP\n";
			break;
		}
		case LOAD_ATTR:
		{
			cout << "LOAD_ATTR ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		case STORE_ATTR:
		{
			cout << "STORE_ATTR ";
			i++;
			IntObject* arg = ((IntObject*)codelist->list[i]);
			cout << arg->val << endl;
			break;
		}
		}
		i++;
	}
	cout << endl;
	for (int i = 0; i < funccodeobj.size(); i++)
	{
		print(funccodeobj[i]);
		cout << endl;
		printcodeobj((CodeObject*)funccodeobj[i]);
		cout << endl;
	}
}

int main(int argc,char* argv[])
{
#ifndef NDEBUG
	char* filename = (char*)"Debug/test.txt";
#else
	if (argc == 1)return 0;
	char* filename = argv[1];
#endif
	ifstream t(filename);
	stringstream buffer;
	buffer << t.rdbuf();
	string content(buffer.str());

	Preprocessor* prep = preprocessor_init();
	string pre_file = preprocessor(prep,content);

	Lexer* lexer = lexer_init(pre_file, filename);
	Parser* parser = parser_init(lexer,prep->blockstyle);
	SyntaxTree* syntaxtree = parser_parse(parser);
#ifndef NDEBUG
	syntaxtree_print(syntaxtree, 0);
#endif
	Object* codeobj = CodeObject_New();
	generator(codeobj, syntaxtree);
	((CodeObject*)codeobj)->code->objattr->obj_additem(((CodeObject*)codeobj)->code, IntObject_NewFromInt(LOAD_VALUE));
	((CodeObject*)codeobj)->code->objattr->obj_additem(((CodeObject*)codeobj)->code, IntObject_NewFromInt(0));
	((CodeObject*)codeobj)->code->objattr->obj_additem(((CodeObject*)codeobj)->code, IntObject_NewFromInt(RETURN));
#ifndef NDEBUG
	printcodeobj((CodeObject*)codeobj);
#endif
	Object* frameobj = FrameObject_NewFrameCodeObject(codeobj,builtins_init());
	frame_eval(frameobj);

	t.close();
}