#pragma once

#include "parser.hpp"
#include "token.hpp"
#include "astgrammar.hpp"
#include "astcut.hpp"
#include "astforced.hpp"
#include "astgather.hpp"
#include "astgroup.hpp"
#include "astnameditem.hpp"
#include "astnameleaf.hpp"
#include "astrhs.hpp"
#include "astnegativelookahead.hpp"
#include "astopt.hpp"
#include "astpositivelookahead.hpp"
#include "astrepeat0.hpp"
#include "astrepeat1.hpp"
#include "astrule.hpp"
#include "aststringleaf.hpp"
#include "astalt.hpp"

/*
元语法
*/
class GrammarParser : Parser
{
public:
    GrammarParser(Lexer *lexer) : Parser(lexer) {}
    void *start();
    AST *grammar();
    AST *rule();
    vector<AST *> rhs();
    AST *alt();
    vector<AST *> items();
    AST *nameditem();
    string annotation();
    AST *item();
    AST *atom();
    AST *lookahead();
};

string GrammarParser::annotation()
{
    if (this->lexer->peek_token()->type == TK_LMIDDLE)
    {
        int mark = this->lexer->mark;
        this->expect(TK_LMIDDLE);
        while (this->lexer->next_token()->type != TK_RMIDDLE)
        {
        }
        string returntype = this->lexer->code.substr(mark, this->lexer->mark - 1 - mark);
        this->expect(TK_RMIDDLE);
        return returntype;
    }
    return "void";
}

void *GrammarParser::start()
{
    return this->grammar();
}

AST *GrammarParser::grammar()
{
    vector<AST *> rules;
    string meta = "";
    if (this->lexer->peek_token()->type == TK_AT)
    {
        this->expect(TK_AT);
        meta = this->lexer->peek_token()->str;
        this->expect(TK_STRING);
    }
    while (this->lexer->peek_token()->type != TK_FILEEND)
    {
        rules.push_back(this->rule());
        // cout << this->lexer->peek_token()->toString() << endl;
    }
    return new ASTGrammar(rules, meta);
}

AST *GrammarParser::rule()
{
    string name = this->lexer->peek_token()->str;
    this->expect(TK_ID);
    string returntype = this->annotation();
    this->expect(TK_COLON);
    vector<AST *> rhs = this->rhs();
    return new ASTRule(name, returntype, rhs);
}

vector<AST *> GrammarParser::rhs()
{
    vector<AST *> res;
    if (this->lexer->peek_token()->type == TK_OR)
    {
        this->expect(TK_OR);
    }
    AST *t = this->alt();
    res.push_back(t);
    while (this->lexer->peek_token()->type == TK_OR)
    {
        this->expect(TK_OR);
        t = this->alt();
        if (t == NULL)
            break;
        res.push_back(t);
    }
    return res;
}

AST *GrammarParser::alt()
{
    vector<AST *> items = this->items();
    string action = "";
    /*获取动作*/
    if (this->lexer->peek_token()->type == TK_LLARGE)
    {
        int mark = this->lexer->mark;
        this->expect(TK_LLARGE);
        while (this->lexer->next_token()->type != TK_RLARGE)
        {
        }
        action = this->lexer->code.substr(mark, this->lexer->mark - 1 - mark);
        this->expect(TK_RLARGE);
    }
    return new ASTAlt(items, action);
}

vector<AST *> GrammarParser::items()
{
    vector<AST *> res;
    AST *t = this->nameditem();
    res.push_back(t);
    while (this->lexer->peek_token()->type != TK_FILEEND)
    {
        t = this->nameditem();
        if (t == NULL)
            break;
        res.push_back(t);
    }
    return res;
}

AST *GrammarParser::nameditem()
{
    if (this->lexer->peek_token()->type == TK_ID)
    {
        int mark = this->lexer->mark;
        TokenInfo *token = this->lexer->peek_token();

        string name = this->lexer->peek_token()->str;
        this->expect(TK_ID);
        string type = this->annotation();
        if (this->lexer->peek_token()->type == TK_COLON)
        {
            this->lexer->mark = mark;
            this->lexer->token = token;
            return NULL;
        }
        if (this->lexer->peek_token()->type != TK_ASSIGN)
        {
            this->lexer->mark = mark;
            this->lexer->token = token;
            goto item;
        }
        this->expect(TK_ASSIGN);
        AST *item = this->item();
        return new ASTNamedItem(name, item, type);
    }
    AST *it = this->lookahead();
    if (it)
    {
        return it;
    }
item:
    return this->item();
}

AST *GrammarParser::lookahead()
{
    if (this->lexer->peek_token()->type == TK_AND)
    {
        this->expect(TK_AND);
        return new ASTPositiveLookahead(this->atom());
    }
    if (this->lexer->peek_token()->type == TK_NOT)
    {
        this->expect(TK_NOT);
        return new ASTNegativeLookahead(this->atom());
    }
    if (this->lexer->peek_token()->type == TK_LDI)
    {
        this->expect(TK_LDI);
        return new ASTCut();
    }
    return NULL;
}

AST *GrammarParser::item()
{
    if (this->lexer->peek_token()->type == TK_LMIDDLE)
    {
        this->expect(TK_LMIDDLE);
        AST *rhs = new ASTRhs(this->rhs());
        this->expect(TK_RMIDDLE);
        return new ASTOpt(rhs);
    }
    AST *atom = this->atom();
    if (this->lexer->peek_token()->type == TK_QUES)
    {
        this->expect(TK_QUES);
        return new ASTOpt(atom);
    }
    if (this->lexer->peek_token()->type == TK_STAR)
    {
        this->expect(TK_STAR);
        return new ASTRepeat0(atom);
    }
    if (this->lexer->peek_token()->type == TK_ADD)
    {
        this->expect(TK_ADD);
        return new ASTRepeat1(atom);
    }
    return atom;
}

AST *GrammarParser::atom()
{
    if (this->lexer->peek_token()->type == TK_LLITTLE)
    {
        this->expect(TK_LLITTLE);
        vector<AST *> rhs = this->rhs();
        this->expect(TK_RLITTLE);
        return new ASTGroup(rhs);
    }
    if (this->lexer->peek_token()->type == TK_ID)
    {
        string name = this->lexer->peek_token()->str;
        this->expect(TK_ID);
        return new ASTNameLeaf(name);
    }
    if (this->lexer->peek_token()->type == TK_STRING)
    {
        string name = this->lexer->peek_token()->str;
        this->expect(TK_STRING);
        return new ASTStringLeaf(name);
    }
    return NULL;
}