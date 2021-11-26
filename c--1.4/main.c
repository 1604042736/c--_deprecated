#include "preprocessor.h"
#include "lexer.h"
#include "parser.h"
#include "translater.h"
#include "namespaceobject.h"
#include "frameobject.h"
#include "eval.h"

struct NamespaceObject* compiler(char* filename,char* namespacename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("无法打开:%s", filename);
		exit(-1);
	}

	struct Preprocessor* preprocessor = Preprocessor_New(file,filename);
	Preprocessor_Preprocess(preprocessor);

	struct Lexer* lexer = Lexer_New(preprocessor->result);

	struct Parser* parser = Parser_New(lexer);
	struct NameSpaceASTObject* astobject = Parser_Parse(parser);

	struct Translater* translater = Translater_New();
	Translater_Translate(translater, astobject);

	struct NamespaceObject* namespaceobj = NamespaceObject_NewWithName(namespacename);
	struct FrameObject* frameobj = FrameObject_NewWithByteCode(translater->bytecode);
	frameobj->locals = namespaceobj->globals;
	Eval(frameobj);

	fclose(file);

	return namespaceobj;
}

int main(int argc,char* argv[])
{
#ifndef NDEBUG
	char* filename = "test.txt";
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		filename = "Debug/test.txt";
		file = fopen(filename, "r");
	}
#else
	if (argc == 1)
	{
		return 0;
	}
	char* filename = argv[1];
	FILE* file = fopen(argv[1], "r");
#endif
	if (file == NULL)
	{
		printf("无法打开:%s", filename);
		return 1;
	}
	Object_Init();

	struct Preprocessor* preprocessor = Preprocessor_New(file,filename);
	Preprocessor_Preprocess(preprocessor);

	Lexer_Init();
	struct Lexer* lexer = Lexer_New(preprocessor);

	struct Parser* parser = Parser_New(lexer);
	struct NameSpaceASTObject* astobject = Parser_Parse(parser);
	printastobject((struct Object*)astobject,0);

	Translater_Init();
	struct Translater* translater = Translater_New();
	Translater_Translate(translater, astobject);
	ByteCodeObject_Print2(translater->bytecode);

	struct NamespaceObject* namespaceobj = NamespaceObject_NewWithName("main");
	struct FrameObject* frameobj = FrameObject_NewWithByteCode(translater->bytecode);
	frameobj->locals = namespaceobj->globals;
	Eval(frameobj);

	fclose(file);

#ifndef NDEBUG
	_CrtDumpMemoryLeaks();
#endif // !NDEBUG
	return 0;
}