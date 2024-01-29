#pragma once
#include <axx/interface/SemanticAnalyzerInterface.hpp>
#include <axx/semantic/SemanticVisitor.hpp>
#include <memory>

class SemanticAnalyzer : public SemanticAnalyzerInterface
{
private:
    std::unique_ptr<SemanticVisitor> visitor;

public:
    SemanticAnalyzer();
    void check(AST *_tree) override;
};
