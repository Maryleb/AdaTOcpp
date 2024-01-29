#include <axx/codegen/CodeEmittingNodeVisitor.hpp>
#include <axx/AST/ASTNode.hpp>
#include <map>

CodeEmittingNodeVisitor::CodeEmittingNodeVisitor(std::ostream& _stream): 
    stream(_stream), block_declarations({}){}

void CodeEmittingNodeVisitor::write(std::string s) {
    std::map<std::string, std::string> reserved = {
        {"Integer", "int"},
        {"String", "std::string"}, 
        {"integer", "int"},
        {"string", "std::string"}, 
    };
    if (reserved.find(s) != reserved.end()) {
        this->stream << reserved.at(s);
    } else {
        this->stream << s;
    }
}

void CodeEmittingNodeVisitor::write(Token token) {
    if (token.getType() == Type::string) {
        write("\"");
        write(token.getValue());
        write("\"");
    } else {
        write(token.getValue());
    }
}

void CodeEmittingNodeVisitor::write(Leaf* leaf) {
    write(leaf->token);
}

void CodeEmittingNodeVisitor::visitLeaf(Leaf *_acceptor)
{
    write(_acceptor);
}

void CodeEmittingNodeVisitor::visitFormalParamsNode(FormalParamsNode *_acceptor)
{
    int count = _acceptor->names.size();
    for (int i = 0; i < count; i++) {
        write(_acceptor->types[i]);
        write(" ");
        write(_acceptor->names[i]);
        if (i != (count - 1))
            write(", ");
    }
}
void CodeEmittingNodeVisitor::visitActualParamsNode(ActualParamsNode *_acceptor)
{
    int count = _acceptor->params.size();
    for (int i = 0; i < count; i++) {
        _acceptor->params[i]->accept(this);
        if (i != (count - 1))
            write(", ");
    }
}
void CodeEmittingNodeVisitor::visitCallNode(CallNode *_acceptor)
{
    write(_acceptor->callable);
    write("(");
    _acceptor->params->accept(this);
    write(")");
}
void CodeEmittingNodeVisitor::visitBinaryNode(BinaryNode *_acceptor)
{
    std::map<Type, std::string> bin_op_strs = {
        {Type::orop, "||"},
        {Type::andop, "&&"},
        {Type::plus, "+"},
        {Type::minus, "-"},
        {Type::star, "*"},
        {Type::div, "/"},
        {Type::greater, ">"},
        {Type::less, "<"},
        {Type::equal, "=="},
        {Type::noteq, "!="},
        {Type::grequal, ">="},
        {Type::lequal, "<="},
        {Type::mod, "%"},
    };
    Type op = _acceptor->op->token.getType();
    write("(");
    _acceptor->left->accept(this);
    write(" ");
    write(bin_op_strs.at(op));
    write(" ");
    _acceptor->right->accept(this);
    write(")");
}
void CodeEmittingNodeVisitor::visitUnaryNode(UnaryNode *_acceptor)
{
    std::map<Type, std::string> bin_op_strs = {
        {Type::notop, "!"},
        {Type::plus, "+"},
        {Type::minus, "-"}
    };
    Type op = _acceptor->op->token.getType();
    write(bin_op_strs.at(op));
    _acceptor->operand->accept(this);
}
void CodeEmittingNodeVisitor::visitAssignmentNode(AssignmentNode *_acceptor)
{
    write(_acceptor->left);
    write(" = ");
    _acceptor->right->accept(this);
}
void CodeEmittingNodeVisitor::visitBlockNode(BlockNode *_acceptor)
{
    write("{\n");
    for (auto declaration: this->block_declarations) {
        declaration->accept(this);
        write(";\n");
    }
    this->block_declarations = {};
    for (auto child: _acceptor->children) {
        child->accept(this);
        write(";\n");
    }
    write("}");
}
void CodeEmittingNodeVisitor::visitProgramNode(ProgramNode *_acceptor)
{
    write("#include <bits/stdc++.h>\n");
    for (auto child: _acceptor->children) {
        child->accept(this);
        write(";\n");
    }
}
void CodeEmittingNodeVisitor::visitFunctionNode(FunctionNode *_acceptor)
{
    write(_acceptor->return_type);
    write(" ");
    write(_acceptor->id);
    write("(");
    _acceptor->formal_params->accept(this);
    write(")\n");
    this->block_declarations = _acceptor->var_declarations;
    _acceptor->body->accept(this);
}
void CodeEmittingNodeVisitor::visitProcedureNode(ProcedureNode *_acceptor)
{
    write("void");
    write(" ");
    write(_acceptor->id);
    write("(");
    _acceptor->formal_params->accept(this);
    write(")\n");
    this->block_declarations = _acceptor->var_declarations;
    _acceptor->body->accept(this);
}
void CodeEmittingNodeVisitor::visitElseNode(ElseNode *_acceptor)
{
    write("else");
    _acceptor->body->accept(this);
}
void CodeEmittingNodeVisitor::visitElifNode(ElifNode *_acceptor)
{
    write("else if");
    write(" ");
    write("(");
    _acceptor->condition->accept(this);
    write(")\n");
    _acceptor->body->accept(this);
    if (_acceptor->next_elif != nullptr) {
        _acceptor->next_elif->accept(this);
    }
    if (_acceptor->next_else != nullptr) {
        _acceptor->next_else->accept(this);
    }
}
void CodeEmittingNodeVisitor::visitIfNode(IfNode *_acceptor)
{
    write("if");
    write(" ");
    write("(");
    _acceptor->condition->accept(this);
    write(")\n");
    _acceptor->body->accept(this);
    if (_acceptor->next_elif != nullptr) {
        _acceptor->next_elif->accept(this);
    }
    if (_acceptor->next_else != nullptr) {
        _acceptor->next_else->accept(this);
    }
}
void CodeEmittingNodeVisitor::visitWhileNode(WhileNode *_acceptor)
{
    write("while");
    write(" ");
    write("(");
    _acceptor->condition->accept(this);
    write(")\n");
    _acceptor->body->accept(this);  
}
void CodeEmittingNodeVisitor::visitForNode(ForNode *_acceptor)
{
    write("for");
    write(" ");
    write("(");
    write("int ");
    write(_acceptor->iterator);
    write(" = ");
    write(_acceptor->from);
    write("; ");
    write(_acceptor->iterator);
    write(" < ");
    write(_acceptor->to);
    write("; ");
    write(_acceptor->iterator);
    write("++");
    write(")\n");
    _acceptor->body->accept(this);  
}
void CodeEmittingNodeVisitor::visitVarDeclNode(VariableDeclarationNode *_acceptor)
{
    write(_acceptor->type);
    write(" ");
    write(_acceptor->var_name);
    if (_acceptor->size != 0) {
        write("[");
        write(std::to_string(_acceptor->size));
        write("]");
    }
}
void CodeEmittingNodeVisitor::visitReturnNode(ReturnNode *_acceptor)
{
    write("return");
    write(" ");
    _acceptor->return_value->accept(this);
}