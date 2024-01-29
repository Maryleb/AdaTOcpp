#include <axx/semantic/SemanticAnalyzer.hpp>

SemanticAnalyzer::SemanticAnalyzer()
{
    visitor = std::make_unique<SemanticVisitor>();
    visitor->stdinit();
}

void SemanticAnalyzer::check(AST* _tree)
{
    _tree->accept(visitor.get());
}
