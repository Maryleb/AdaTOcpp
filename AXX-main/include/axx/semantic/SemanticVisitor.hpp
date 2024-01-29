#pragma once
#include <axx/interface/NodeVisitorInterface.hpp>
#include <axx/token/Token.hpp>
#include <axx/semantic/Symbol.hpp>

#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <map>
#include <vector>

class SemanticVisitor : public NodeVisitorInterface
{
private:
    typedef std::map<std::string, Symbol> localtable_t;
    typedef std::set<std::string> typeset_t;
    typedef typeset_t::const_iterator type_t;
    typedef std::pair<type_t, std::vector<type_t>> func_pair_t;
    typedef std::map<std::string, func_pair_t> func_map_t;

    std::stack<std::unique_ptr<localtable_t>> symtable;
    typeset_t set;
    func_map_t funcs;
    type_t evaluated_type;
    unsigned int lastpos;
    unsigned int lastrow;
public:
    void visitLeaf(Leaf *_acceptor);
    void visitFormalParamsNode(FormalParamsNode *_acceptor);
    void visitActualParamsNode(ActualParamsNode *_acceptor);
    void visitCallNode(CallNode *_acceptor);
    void visitBinaryNode(BinaryNode *_acceptor);
    void visitUnaryNode(UnaryNode *_acceptor);
    void visitAssignmentNode(AssignmentNode *_acceptor);
    void visitReturnNode(ReturnNode *_acceptor);
    void visitBlockNode(BlockNode *_acceptor);
    void visitProgramNode(ProgramNode *_acceptor);
    void visitFunctionNode(FunctionNode *_acceptor);
    void visitProcedureNode(ProcedureNode *_acceptor);
    void visitElseNode(ElseNode *_acceptor);
    void visitElifNode(ElifNode *_acceptor);
    void visitIfNode(IfNode *_acceptor);
    void visitWhileNode(WhileNode *_acceptor);
    void visitForNode(ForNode *_acceptor);
    void visitVarDeclNode(VariableDeclarationNode *_acceptor);

    void stdinit();
};