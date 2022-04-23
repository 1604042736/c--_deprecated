#pragma once

#include "ast.hpp"

class ASTGather : public AST
{
public:
    ASTGather(AST*, AST*);
    AST* sep;
    AST* node;
};

ASTGather::ASTGather(AST* sep, AST* node)
{
    this->sep = sep;
    this->node = node;
}