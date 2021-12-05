#include "preprocessor.h"
#include "lexer.h"
#include "parser.h"
#include "translater.h"
#include "namespaceobject.h"
#include "frameobject.h"
#include "eval.h"
#include "builtins.h"
#include "namespace_system.h"
#include "memory.h"

struct NamespaceObject* compiler(char* filename,char* namespacename,int print)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("无法打开:%s", filename);
		exit(-1);
	}

	struct Preprocessor* preprocessor = Preprocessor_New(file,filename);
	Preprocessor_Preprocess(preprocessor);

	struct Lexer* lexer = Lexer_New(preprocessor);

	struct Parser* parser = Parser_New(lexer);
	struct NameSpaceASTObject* astobject = Parser_Parse(parser);
	if (print)
	{
		printastobject((struct Object*)astobject, 0);
	}

	struct Translater* translater = Translater_New();
	Translater_Translate(translater, astobject);
	if (print)
	{
		ByteCodeObject_Print2(translater->bytecode);
	}

	struct NamespaceObject* namespaceobj = NamespaceObject_NewWithName(namespacename);
	struct FrameObject* frameobj = FrameObject_NewWithByteCode(translater->bytecode);
	frameobj->locals = namespaceobj->globals;
	frameobj->globals = builtins;
	if (Eval(frameobj) == NULL)
	{
		if (exception != NULL)
		{
			if (exception->filename == NULL)
			{
				exception->filename = StringObject_NewWithString(filename);
			}
			printf("\n%s(%d,%d):错误:", exception->filename->string, exception->lineno, exception->linepos);
			printf("%s\n", exception->message->string);
			exit(-1);
		}
		else
		{
			printf("未知错误\n");
			exit(-1);
		}
	}

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
		filename = "../test.txt";
	}
#else
	if (argc == 1)
	{
		return 0;
	}
	char* filename = argv[1];
#endif
	Memory_Init();
	Namespace_System_Init();
	Object_Init();
	Lexer_Init();
	Translater_Init();
	Builtins_Init();

#ifndef NDEBUG
	compiler(filename, "main", 1);
#else
	compiler(filename, "main", 0);
#endif
	return 0;
}