#pragma once
#include <axx/interface/ASTNodeInterface.hpp>
#include <axx/interface/VisitableNodeInterface.hpp>
#include <axx/token/Token.hpp>
#include <vector>

class BaseASTNode : public ASTNodeInterface, public VisitableNodeInterface
{
};

// ============= Declaration =============
class VariableDeclarationNode : public BaseASTNode {
public:
    Token var_name;
    Leaf* type;
    int size;
    VariableDeclarationNode(Token var_name, Leaf* type, int size = 0);
    void print(int indent) override;
    void accept(NodeVisitorInterface *_visitor) override;
};


// ============= Expressions =============
class ExpressionNode : public BaseASTNode
{
}; // Абстрактный класс выражения

class Leaf : public ExpressionNode
{
public:
    void print(int indent) override;
    Token token;
    Leaf(Token token);
    void accept(NodeVisitorInterface *_visitor) override;
};

// ============= Params =============
class FormalParamsNode : public BaseASTNode
{
public:
    void print(int indent) override;
    std::vector<Leaf *> names;
    std::vector<Leaf *> types;
    FormalParamsNode(std::vector<Leaf *> params, std::vector<Leaf *> types);
    void add_param(Leaf *name, Leaf *type);
    void accept(NodeVisitorInterface *_visitor) override;
};

class ActualParamsNode : public BaseASTNode
{
public:
    void print(int indent) override;
    std::vector<ExpressionNode *> params;
    ActualParamsNode(std::vector<ExpressionNode *> params);
    void add_child(ExpressionNode *param);
    void accept(NodeVisitorInterface *_visitor) override;
};

// ============= Expressions =============
class CallNode : public ExpressionNode
{
public:
    void print(int indent) override;
    Token callable;
    ActualParamsNode *params;
    CallNode(Token callable, ActualParamsNode *params);
    void accept(NodeVisitorInterface *_visitor) override;
};
class BinaryNode : public ExpressionNode
{
public:
    void print(int indent) override;
    ExpressionNode *left;
    Leaf *op;
    ExpressionNode *right;
    BinaryNode(ExpressionNode *left, Leaf *op, ExpressionNode *right);
    void accept(NodeVisitorInterface *_visitor) override;
};

class UnaryNode : public ExpressionNode
{
public:
    void print(int indent) override;
    Leaf *op;
    ExpressionNode *operand;
    UnaryNode(Leaf *op, ExpressionNode *operand);
    void accept(NodeVisitorInterface *_visitor) override;
};

class AssignmentNode : public BaseASTNode
{
public:
    void print(int indent) override;
    Leaf *left;
    ExpressionNode *right;
    AssignmentNode(Leaf *left, ExpressionNode *right);
    void accept(NodeVisitorInterface *_visitor) override;
};

class ReturnNode : public BaseASTNode
{
public:
    void print(int indent) override;
    ExpressionNode *return_value;
    ReturnNode(ExpressionNode *return_value);
    void accept(NodeVisitorInterface *_visitor) override;
};

// ============= Block =============

class BlockNode : public BaseASTNode
{
public:
    void print(int indent) override;
    std::vector<BaseASTNode *> children;
    BlockNode(std::vector<BaseASTNode *> children);
    BlockNode();

    void add_child(BaseASTNode *child);
    void accept(NodeVisitorInterface *_visitor) override;
};

class ProgramNode : public BlockNode
{
    void print(int indent) override;
    void accept(NodeVisitorInterface *_visitor) override;
};

// ============= Compound statements =============

class FunctionNode : public BaseASTNode
{
public:
    void print(int indent) override;
    Leaf *id;
    Leaf *return_type;
    FormalParamsNode *formal_params;
    BlockNode *body;
    std::vector<VariableDeclarationNode*> var_declarations;
    FunctionNode(Leaf *id, FormalParamsNode *formal_params, Leaf *return_type, BlockNode *body, std::vector<VariableDeclarationNode*> var_declarations);
    void accept(NodeVisitorInterface *_visitor) override;
};

class ProcedureNode : public BaseASTNode
{
public:
    void print(int indent) override;
    Leaf *id;
    FormalParamsNode *formal_params;
    BlockNode *body;
    std::vector<VariableDeclarationNode*> var_declarations;
    ProcedureNode(Leaf *id, FormalParamsNode *formal_params, BlockNode *body, std::vector<VariableDeclarationNode*> var_declarations);
    void accept(NodeVisitorInterface *_visitor) override;
};

class ElseNode : public BaseASTNode
{
public:
    void print(int indent) override;
    BlockNode *body;
    ElseNode(BlockNode *body);
    void accept(NodeVisitorInterface *_visitor) override;
};

class ElifNode : public BaseASTNode
{
public:
    void print(int indent) override;
    ExpressionNode *condition;
    BlockNode *body;
    ElifNode *next_elif;
    ElseNode *next_else;
    ElifNode(ExpressionNode *condition, BlockNode *body);
    void accept(NodeVisitorInterface *_visitor) override;
};

class IfNode : public BaseASTNode
{
public:
    void print(int indent) override;
    ExpressionNode *condition;
    BlockNode *body;
    ElifNode *next_elif;
    ElseNode *next_else;
    IfNode(ExpressionNode *condition, BlockNode *body);
    void accept(NodeVisitorInterface *_visitor) override;
};

class WhileNode : public BaseASTNode
{
public:
    void print(int indent) override;
    ExpressionNode *condition;
    BlockNode *body;
    WhileNode(ExpressionNode *condition, BlockNode *body);
    void accept(NodeVisitorInterface *_visitor) override;
};

class ForNode : public BaseASTNode
{
public:
    void print(int indent) override;
    Leaf *iterator;
    Leaf *from;
    Leaf *to;
    BlockNode *body;
    ForNode(Leaf *iterator, Leaf *from, Leaf *to, BlockNode *body);
    void accept(NodeVisitorInterface *_visitor) override;
};

void print_indented_line(std::string text, int indent);
