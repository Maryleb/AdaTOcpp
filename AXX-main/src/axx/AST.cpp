#include <axx/AST/AST.hpp>

AST::AST(BaseASTNode *root)
{
    this->root = root;
}

void AST::print()
{
    this->root->print(0);
}

void AST::accept(NodeVisitorInterface *_visitor)
{
    root->accept(_visitor);
}
