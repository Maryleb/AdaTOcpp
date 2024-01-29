#pragma once
#include <axx/interface/ParserInterface.hpp>
#include <axx/AST/ASTNode.hpp>
#include <axx/AST/AST.hpp>
#include <vector>

class Parser : public ParserInterface
{
private:
    LexerInterface* lexer;
    Token token;  // Текущий токен кода
    std::vector<Token> future_tokens; // Сюда будут складываться токены при просмотре "наперёд" методом forward

    bool is_token_in_firsts(std::string grammar_node);
    bool token_matches_any(std::vector<Type> types);
    bool token_matches(Type type);
    void error(std::string message);
    void next_token();
    Token forward(int k);
    Token get_token();
    Token check_get_next(Type type);

    ProgramNode * program();
    void statements(BlockNode *parent_block);
    void statement(BlockNode *parent_block);
    void root_stmt(BlockNode *parent_block);
    FunctionNode * function_declaration();
    ProcedureNode * procedure_declaration();
    FormalParamsNode * formal_params();
    BlockNode * block();
    void nested_stmt(BlockNode *parent_block);
    void compound_stmt(BlockNode *parent_block);
    IfNode * if_stmt();
    ElifNode * elsif_stmt();
    ElseNode * else_block();
    WhileNode * while_stmt();
    ForNode * for_stmt();
    void simple_stmt(BlockNode *parent_block);
    AssignmentNode * assignment();
    ReturnNode * return_stmt();
    ExpressionNode * expression();
    ExpressionNode * disjunction();
    ExpressionNode * conjunction();
    ExpressionNode * inversion();
    ExpressionNode * comparison();
    ExpressionNode * sum();
    ExpressionNode * term();
    ExpressionNode * factor();
    ExpressionNode * primary();
    ActualParamsNode * func_call();
    Leaf * atom();
    ActualParamsNode * actual_params();
    std::vector<VariableDeclarationNode*> variable_declarations();
    VariableDeclarationNode* variable_declaration();

public:
    void setLexer(LexerInterface*);
    AST* getAST();
    Parser();
};