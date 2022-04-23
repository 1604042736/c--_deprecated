/*
Pegen解析器的主程序
*/
#include <iostream>
#include "lexer.hpp"
#include "pegen.hpp"
#include "grammar_parser.hpp"
using namespace std;

/*测试Lexer*/
void test_lexer(Lexer *lexer)
{
    TokenInfo *token = lexer->peek_token();
    while (token->type != TK_FILEEND)
    {
        cout << token->toString() << endl;
        token = lexer->next_token();
    }
}

int main(int argc, char *argv[])
{
#ifndef DEBUG
    char *filename = "test.gram";
#else
    if (argc == 1)
    {
        cout << "Usage: " << argv[0] << " filename" << endl;
        return 0;
    }
    char *filename = argv[1];
#endif
    Lexer *lexer = new Lexer(filename);
    GrammarParser *parser = new GrammarParser(lexer);
    AST *ast = (AST *)parser->start();
    ast->print();
    pegen = new Pegen("test.hpp");
    ast->gen();
}