#pragma once
#include <axx/AST/ASTNode.hpp>
#include <axx/interface/NodeVisitorInterface.hpp>

class AST
{
    BaseASTNode *root;

public:
    AST(BaseASTNode *root);
    void print();
    void accept(NodeVisitorInterface *_visitor);
};
