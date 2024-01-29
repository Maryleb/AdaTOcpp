#include <axx/AST/ASTNode.hpp>
#include <iostream>

Leaf::Leaf(Token token) : token(token) {}

ActualParamsNode::ActualParamsNode(std::vector<ExpressionNode *> params) : params(params) {}

void ActualParamsNode::add_child(ExpressionNode *param)
{
    this->params.push_back(param);
}

CallNode::CallNode(Token callable, ActualParamsNode *params) : callable(callable), params(params) {}

BinaryNode::BinaryNode(ExpressionNode *left, Leaf *op, ExpressionNode *right) : left(left), op(op), right(right) {}

UnaryNode::UnaryNode(Leaf *op, ExpressionNode *operand) : op(op), operand(operand) {}

AssignmentNode::AssignmentNode(Leaf *left, ExpressionNode *right) : left(left), right(right) {}

ReturnNode::ReturnNode(ExpressionNode *return_value) : return_value(return_value) {}

BlockNode::BlockNode(std::vector<BaseASTNode *> children) : children(children) {}

BlockNode::BlockNode() : children({}) {}

FunctionNode::FunctionNode(Leaf *id, FormalParamsNode *formal_params, Leaf *return_type, BlockNode *body, std::vector<VariableDeclarationNode*> var_declarations)
    : id(id), formal_params(formal_params), return_type(return_type), body(body), var_declarations(var_declarations) {}

ProcedureNode::ProcedureNode(Leaf *id, FormalParamsNode *formal_params, BlockNode *body, std::vector<VariableDeclarationNode*> var_declarations)
    : id(id), formal_params(formal_params), body(body), var_declarations(var_declarations) {}

IfNode::IfNode(ExpressionNode *condition, BlockNode *body)
    : condition(condition), body(body)
{
    this->next_elif = nullptr;
    this->next_else = nullptr;
}

ElifNode::ElifNode(ExpressionNode *condition, BlockNode *body) : condition(condition), body(body)
{
    this->next_elif = nullptr;
    this->next_else = nullptr;
}

ElseNode::ElseNode(BlockNode *body) : body(body) {}

WhileNode::WhileNode(ExpressionNode *condition, BlockNode *body) : condition(condition), body(body) {}

ForNode::ForNode(Leaf *iterator, Leaf *from, Leaf *to, BlockNode *body)
    : iterator(iterator), from(from), to(to), body(body) {}

void BlockNode::add_child(BaseASTNode *child)
{
    this->children.push_back(child);
}

FormalParamsNode::FormalParamsNode(std::vector<Leaf *> params, std::vector<Leaf *> types) : names(params), types(types) {}

void FormalParamsNode::add_param(Leaf *name, Leaf *type)
{
    this->names.push_back(name);
    this->types.push_back(type);
}

VariableDeclarationNode::VariableDeclarationNode(Token var_name, Leaf* type, int size)
    : var_name(var_name), type(type), size(size) {}


void print_indented_line(std::string text, int indent)
{
    std::string indent_str = "   |";
    for (int i = 0; i < indent; i++)
    {
        std::cout << indent_str;
    }
    std::cout << text;
    std::cout << "\n";
}

void Leaf::print(int indent)
{
    std::string text = "<" + type_to_str(this->token.getType()) + ", " + this->token.getValue() + ">";
    print_indented_line(text, indent);
}

void FormalParamsNode::print(int indent)
{
    std::string text = "FormalParams";
    print_indented_line(text, indent);
    print_indented_line("names:", indent + 1);
    for (auto name : this->names)
    {
        name->print(indent + 2);
    }
    print_indented_line("types:", indent + 1);
    for (auto type : this->types)
    {
        type->print(indent + 2);
    }
}

void ActualParamsNode::print(int indent)
{
    std::string text = "ActualParams";
    print_indented_line(text, indent);
    for (auto param : this->params)
    {
        param->print(indent + 1);
    }
}

void CallNode::print(int indent)
{
    std::string text = "Call";
    print_indented_line(text, indent);
    print_indented_line("callable:", indent + 1);
    print_indented_line(this->callable.getValue(), indent + 2);
    print_indented_line("params:", indent + 1);
    this->params->print(indent + 2);
}

void BinaryNode::print(int indent)
{
    std::string text = "BinaryOp (" + type_to_str(this->op->token.getType()) + ")";
    print_indented_line(text, indent);
    this->left->print(indent + 1);
    this->right->print(indent + 1);
}

void UnaryNode::print(int indent)
{
    std::string text = "UnaryOp (" + type_to_str(this->op->token.getType()) + ")";
    print_indented_line(text, indent);
    this->operand->print(indent + 1);
}

void AssignmentNode::print(int indent)
{
    std::string text = "Assignment";
    print_indented_line(text, indent);
    this->left->print(indent + 1);
    this->right->print(indent + 1);
}

void ReturnNode::print(int indent)
{
    std::string text = "Return";
    print_indented_line(text, indent);
    this->return_value->print(indent + 1);
}

void BlockNode::print(int indent)
{
    std::string text = "Block";
    print_indented_line(text, indent);
    for (auto child : this->children)
    {
        child->print(indent + 1);
    }
}

void ProgramNode::print(int indent)
{
    std::string text = "Program";
    print_indented_line(text, indent);
    for (auto child : this->children)
    {
        child->print(indent + 1);
    }
}

void FunctionNode::print(int indent)
{
    std::string text = "Function declaration";
    print_indented_line(text, indent);
    print_indented_line("name:", indent + 1);
    this->id->print(indent + 2);
    print_indented_line("return type:", indent + 1);
    this->return_type->print(indent + 2);
    this->formal_params->print(indent + 1);
    print_indented_line("variable declarations:", indent + 1);
    for (auto declaration: this->var_declarations) {
        declaration->print(indent + 2);
    }
    this->body->print(indent + 1);
}

void ProcedureNode::print(int indent)
{
    std::string text = "Procedure declaration";
    print_indented_line(text, indent);
    print_indented_line("name:", indent + 1);
    this->id->print(indent + 2);
    this->formal_params->print(indent + 1);
    print_indented_line("variable declarations:", indent + 1);
    for (auto declaration: this->var_declarations) {
        declaration->print(indent + 2);
    }
    this->body->print(indent + 1);
}

void ElseNode::print(int indent)
{
    std::string text = "Else";
    print_indented_line(text, indent);
    this->body->print(indent + 1);
}

void ElifNode::print(int indent)
{
    std::string text = "Elif";
    print_indented_line(text, indent);
    print_indented_line("condition:", indent + 1);
    this->condition->print(indent + 2);
    this->body->print(indent + 1);
    if (this->next_elif != nullptr)
    {
        this->next_elif->print(indent + 1);
    }
    else if (this->next_else != nullptr)
    {
        this->next_else->print(indent + 1);
    }
}

void IfNode::print(int indent)
{
    std::string text = "If";
    print_indented_line(text, indent);
    print_indented_line("condition:", indent + 1);
    this->condition->print(indent + 2);
    this->body->print(indent + 1);
    if (this->next_elif != nullptr)
    {
        this->next_elif->print(indent + 1);
    }
    else if (this->next_else != nullptr)
    {
        this->next_else->print(indent + 1);
    }
}

void WhileNode::print(int indent)
{
    std::string text = "While";
    print_indented_line(text, indent);
    print_indented_line("condition:", indent + 1);
    this->condition->print(indent + 2);
    this->body->print(indent + 1);
}

void ForNode::print(int indent)
{
    std::string text = "For";
    print_indented_line(text, indent);
    print_indented_line("iterator:", indent + 1);
    this->iterator->print(indent + 2);
    print_indented_line("from:", indent + 1);
    this->from->print(indent + 2);
    print_indented_line("to:", indent + 1);
    this->to->print(indent + 2);
    this->body->print(indent + 1);
}

void VariableDeclarationNode::print(int indent) {
    std::string text = "Variable Declaration";
    print_indented_line(text, indent);
    print_indented_line("name:", indent + 1);
    print_indented_line(this->var_name.getValue(), indent + 2);
    print_indented_line("type:", indent + 1);
    print_indented_line(this->type->token.getValue(), indent + 2);
    if (this->size != 0) {
        print_indented_line("size:", indent + 1);
        print_indented_line(std::to_string(this->size), indent + 2);
    }
}

void Leaf::accept(NodeVisitorInterface *_visitor) { _visitor->visitLeaf(this); }
void FormalParamsNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitFormalParamsNode(this); }
void ActualParamsNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitActualParamsNode(this); }
void CallNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitCallNode(this); }
void BinaryNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitBinaryNode(this); }
void UnaryNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitUnaryNode(this); }
void AssignmentNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitAssignmentNode(this); }
void ReturnNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitReturnNode(this); }
void BlockNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitBlockNode(this); }
void ProgramNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitProgramNode(this); }
void FunctionNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitFunctionNode(this); }
void ProcedureNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitProcedureNode(this); }
void ElseNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitElseNode(this); }
void ElifNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitElifNode(this); }
void IfNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitIfNode(this); }
void WhileNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitWhileNode(this); }
void ForNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitForNode(this); }
void VariableDeclarationNode::accept(NodeVisitorInterface *_visitor) { _visitor->visitVarDeclNode(this); }

void print_indented_line(std::string text, int indent);
