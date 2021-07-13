#include <stdio.h>
#include <stdlib.h>

#include "Include/preprocessor.h"
#include "Include/lexer.h"
#include "Include/globals.h"
#include "Include/parser.h"
#include "Include/symbol.h"
#include "Include/analyzer.h"

#define DEBUG

#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
#ifdef DEBUG
	FILE* file = fopen("Debug\\test.txt", "r");
	char* filename = "test.txt";
	if (NULL == file)
	{
		file = fopen("test.txt", "r");
	}
#else
	if (argc == 1)
	{
		printf("未传入文件");
		return 1;
	}
	char* filename = argv[1];
	FILE* file = fopen(argv[1], "r");
#endif

	if (NULL == file)
	{
		printf("无法打开文件%s", filename);
		return 1;
	}
	blockstyle = BRACES;
	char* pre_file = preprocessor(file);
	struct Lexer* lexer = lexer_init(pre_file, filename);
	struct Parser* parser = parser_init(lexer);
	struct SyntaxTree* syntaxtree=parser_parse(parser);
	syntaxtree_print(syntaxtree, 0);
	symbol_print(parser->symbol);
	struct Analyzer* analyzer = analyzer_init(syntaxtree, parser->symbol);
	analyzer_check(analyzer, syntaxtree, filename);
	fclose(file);
	return 0;
}