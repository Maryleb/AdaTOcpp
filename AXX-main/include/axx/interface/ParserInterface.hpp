#pragma once
#include <axx/interface/LexerInterface.hpp>
#include <axx/AST/AST.hpp>

class ParserInterface
{
public:
    virtual void setLexer(LexerInterface*) = 0;
    virtual AST* getAST() = 0;
    virtual ~ParserInterface() = default;
};
