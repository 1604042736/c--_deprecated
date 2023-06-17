#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "symtab.h"
#include "generator.h"

int main(int argc,char* argv[])
{
#ifdef _DEBUG
	char* infilename = "test.txt";
	char* outfilename = "test.asm";
#else
	if (argc <= 2)
	{
		printf("Too few args");
		exit(2);
	}
	char* infilename = argv[1];
	char* outfilename = argv[2];
#endif
	struct Lexer* lexer = lexer_new(infilename);
	struct Parser* parser = parser_new(lexer);
	struct AST* ast = parser_start(parser);
	struct Symtab* symtab = symtab_new();
	symtab_typecheck(symtab,ast);

	ast_print(ast, 0);
	printf("\n");
	symtab_print(symtab);
	printf("\n");

	struct Generator* generator = generator_new(outfilename);
	generator_generate(generator, symtab, ast);
}