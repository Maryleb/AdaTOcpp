#pragma once

#include <axx/interface/CodeGeneratorInterface.hpp>
#include <axx/codegen/CodeEmittingNodeVisitor.hpp>

#include <memory>
#include <ostream>

class CodeGenerator : public CodeGeneratorInterface
{
private:
    std::unique_ptr<CodeEmittingNodeVisitor> visitor;

public:
    CodeGenerator(std::ostream& _stream);
    void generate(AST *_ast);
};
