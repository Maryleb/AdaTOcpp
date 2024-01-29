#include <axx/parser/Parser.hpp>
#include <axx/token/Token.hpp>
#include <axx/AST/ASTNode.hpp>
#include <axx/AST/AST.hpp>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>

// Первые терминалы, которые можно встретить, переходя вглубь нетерминалов грамматики
std::map<std::string, std::set<Type>> FIRSTS = {
    {"procedure_declaration", {Type::procedurekw}},
    {"nested_stmt", {Type::lpr, Type::whilekw, Type::plus, Type::ifkw, Type::minus, Type::id, Type::number, Type::forkw, Type::notop, Type::string, Type::returnkw}},
    {"block", {Type::lpr, Type::whilekw, Type::plus, Type::ifkw, Type::minus, Type::id, Type::number, Type::forkw, Type::notop, Type::string, Type::returnkw}},
    {"compound_stmt", {Type::ifkw, Type::whilekw, Type::forkw}},
    {"term", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::string}},
    {"conjunction", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::notop, Type::string}},
    {"variable_declaration", {Type::id}},
    {"if_stmt", {Type::ifkw}},
    {"primary", {Type::lpr, Type::id, Type::number, Type::string}},
    {"sum", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::string}},
    {"function_declaration", {Type::functionkw}},
    {"expression", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::notop, Type::string}},
    {"comparison", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::string}},
    {"return_stmt", {Type::returnkw}},
    {"while_stmt", {Type::whilekw}},
    {"statement", {Type::lpr, Type::whilekw, Type::plus, Type::procedurekw, Type::ifkw, Type::minus, Type::functionkw, Type::id, Type::number, Type::forkw, Type::notop, Type::string, Type::returnkw}},
    {"simple_stmt", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::notop, Type::string, Type::returnkw}},
    {"inversion", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::notop, Type::string}},
    {"variable_declarations", {Type::id}},
    {"actual_params", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::notop, Type::string}},
    {"root_stmt", {Type::functionkw, Type::procedurekw}},
    {"for_stmt", {Type::forkw}},
    {"program", {Type::lpr, Type::whilekw, Type::plus, Type::procedurekw, Type::ifkw, Type::minus, Type::functionkw, Type::id, Type::number, Type::forkw, Type::eof, Type::notop, Type::string, Type::returnkw}},
    {"else_block", {Type::elsekw}},
    {"elsif_stmt", {Type::elsifkw}},
    {"formal_params", {Type::id}},
    {"disjunction", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::notop, Type::string}},
    {"func_call", {Type::lpr}},
    {"atom", {Type::id, Type::number, Type::string}},
    {"assignment", {Type::id}},
    {"factor", {Type::lpr, Type::plus, Type::minus, Type::id, Type::number, Type::string}},
    {"statements", {Type::lpr, Type::whilekw, Type::plus, Type::procedurekw, Type::ifkw, Type::minus, Type::functionkw, Type::id, Type::number, Type::forkw, Type::notop, Type::string, Type::returnkw}}};

Parser::Parser() : token(Token("", Type::id)){};

void Parser::setLexer(LexerInterface *lexer)
{
    this->lexer = lexer;
    this->next_token();
}

AST *Parser::getAST()
{
    return new AST(this->program());
}

bool Parser::is_token_in_firsts(std::string grammar_node)
{
    // Возвращает True, если текущий токен кода можно получить, спускаясь по узлу грамматика grammar_node
    return FIRSTS[grammar_node].find(this->token.getType()) != FIRSTS[grammar_node].end();
}

bool Parser::token_matches_any(std::vector<Type> types)
{
    // Возвращает True, если текущий токен кода имеет тип, имеющийся в types
    for (Type type : types)
    {
        if (this->get_token().getType() == type)
        {
            return true;
        }
    }
    return false;
}

bool Parser::token_matches(Type type)
{
    // Возвращает True, если текущий токен кода имеет тип type
    return this->get_token().getType() == type;
}

void Parser::error(std::string message)
{
    std::string pos = std::to_string(this->get_token().getPos());
    std::string row = std::to_string(this->get_token().getRow());
    std::string value = this->get_token().getValue();
    std::string type = type_to_str(this->get_token().getType());
    message += " pos=" + pos + " row=" + row + " type=" + type + " value=" + value;
    std::cout << message << std::endl;
    throw std::runtime_error(message);
}

void Parser::next_token()
{
    if (this->future_tokens.empty())
    {
        this->token = this->lexer->getToken();
    }
    else
    {
        // Забираем токен из списка будущих токенов
        this->token = this->future_tokens[0];
        this->future_tokens.erase(this->future_tokens.begin());
    }
}

Token Parser::forward(int k)
{
    while (this->future_tokens.size() < k)
    {
        this->future_tokens.push_back(this->lexer->getToken());
    }
    return this->future_tokens[k - 1];
}

Token Parser::get_token()
{
    return this->token;
}

Token Parser::check_get_next(Type type)
{
    // Проверяет, что текущий токен имеет тип type, возвращает его и сразу получает следующий токен
    if (this->token.getType() != type)
    {
        this->error("Unexpected token");
    }
    Token tmp = this->token;
    this->next_token();
    return tmp;
}

ProgramNode *Parser::program()
{
    /*
    program:
        | statements EOF
        | EOF
     */
    ProgramNode *file = new ProgramNode();
    if (this->is_token_in_firsts("statements"))
    {
        this->statements(file);
    }
    file->add_child(new Leaf(this->check_get_next(Type::eof)));
    return file;
};

void Parser::statements(BlockNode *parent_block)
{
    /*
    statements:
        | statement statements
        | statement
     */
    while (this->is_token_in_firsts("statements"))
    {
        this->statement(parent_block);
    }
};

void Parser::statement(BlockNode *parent_block)
{
    /*
    statement:
        | root_stmt
        | nested_stmt
     */
    if (this->is_token_in_firsts("root_stmt"))
    {
        this->root_stmt(parent_block);
    }
    else if (this->is_token_in_firsts("nested_stmt"))
    {
        this->nested_stmt(parent_block);
    }
    else
    {
        this->error("statement");
    }
}

void Parser::root_stmt(BlockNode *parent_block)
{
    /*
    root_stmt:
        | function_declaration SEMICOLON
        | procedure_declaration SEMICOLON
     */
    if (this->is_token_in_firsts("function_declaration"))
    {
        parent_block->add_child(this->function_declaration());
    }
    else if (this->is_token_in_firsts("procedure_declaration"))
    {
        parent_block->add_child(this->procedure_declaration());
    }
    else
    {
        this->error("statement");
    }
    this->check_get_next(Type::semicolon);
}

FunctionNode *Parser::function_declaration()
{
    /*
    function_declaration:
        | FUNCTIONKW ID LPR formal_params RPR RETURNKW ID IS variable_declarations BEGINKW block ENDKW ID
        | FUNCTIONKW ID LPR RPR RETURNKW ID IS variable_declarations BEGINKW block ENDKW ID
     */
    this->check_get_next(Type::functionkw);
    Leaf *id = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::lpr);
    FormalParamsNode *formal_params;
    if (this->is_token_in_firsts("formal_params"))
    {
        formal_params = this->formal_params();
    }
    else
    {
        formal_params = new FormalParamsNode({}, {});
    }
    this->check_get_next(Type::rpr);
    this->check_get_next(Type::returnkw);
    Leaf *return_type = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::is);
    auto declarations = this->variable_declarations();
    this->check_get_next(Type::beginkw);
    BlockNode *body = this->block();
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::id);
    return new FunctionNode(id, formal_params, return_type, body, declarations);
};

ProcedureNode *Parser::procedure_declaration()
{
    /*
    procedure_declaration:
        | PROCEDUREKW ID LPR formal_params RPR IS variable_declarations BEGINKW block ENDKW ID
        | PROCEDUREKW ID LPR RPR IS variable_declarations BEGINKW block ENDKW ID
     */
    this->check_get_next(Type::procedurekw);
    Leaf *id = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::lpr);
    FormalParamsNode *formal_params;
    if (this->is_token_in_firsts("formal_params"))
    {
        formal_params = this->formal_params();
    }
    else
    {
        formal_params = new FormalParamsNode({}, {});
    }
    this->check_get_next(Type::rpr);
    this->check_get_next(Type::is);
    auto declarations = this->variable_declarations();
    this->check_get_next(Type::beginkw);
    BlockNode *body = this->block();
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::id);
    return new ProcedureNode(id, formal_params, body, declarations);
}

FormalParamsNode *Parser::formal_params()
{
    /*
    formal_params:
        | ID COLON ID SEMICOLON formal_params
        | ID COLON ID
     */
    FormalParamsNode *params = new FormalParamsNode({}, {});
    while (this->is_token_in_firsts("formal_params"))
    {
        Leaf *name = new Leaf(this->check_get_next(Type::id));
        this->check_get_next(Type::colon);
        Leaf *type = new Leaf(this->check_get_next(Type::id));
        params->add_param(name, type);
        if (this->get_token().getType() == Type::semicolon)
        {
            this->next_token();
        }
    }
    return params;
};

BlockNode *Parser::block()
{
    /*
    block:
        | nested_stmt block
        | nested_stmt
     */
    BlockNode *block = new BlockNode();
    while (this->is_token_in_firsts("block"))
    {
        this->nested_stmt(block);
    }
    return block;
}

void Parser::nested_stmt(BlockNode *parent_block)
{
    /*
    nested_stmt:
        | compound_stmt SEMICOLON
        | simple_stmt SEMICOLON
     */
    if (this->is_token_in_firsts("compound_stmt"))
    {
        this->compound_stmt(parent_block);
    }
    else if (this->is_token_in_firsts("simple_stmt"))
    {
        this->simple_stmt(parent_block);
    }
    this->check_get_next(Type::semicolon);
}

void Parser::compound_stmt(BlockNode *parent_block)
{
    /*
    compound_stmt:
        | if_stmt
        | for_stmt
        | while_stmt
     */
    if (this->is_token_in_firsts("if_stmt"))
    {
        parent_block->add_child(this->if_stmt());
    }
    else if (this->is_token_in_firsts("for_stmt"))
    {
        parent_block->add_child(this->for_stmt());
    }
    else if (this->is_token_in_firsts("while_stmt"))
    {
        parent_block->add_child(this->while_stmt());
    }
    else
    {
        this->error("compound_stmt");
    }
};

IfNode *Parser::if_stmt()
{
    /*
    if_stmt:
        | IFKW expression THENKW block elsif_stmt ENDKW IFKW
        | IFKW expression THENKW block else_block ENDKW IFKW
        | IFKW expression THENKW block ENDKW IFKW
     */
    this->check_get_next(Type::ifkw);
    ExpressionNode *condition = this->expression();
    this->check_get_next(Type::thenkw);
    BlockNode *body = this->block();
    IfNode *if_node = new IfNode(condition, body);
    if (this->is_token_in_firsts("elsif_stmt"))
    {
        if_node->next_elif = this->elsif_stmt();
    }
    else if (this->is_token_in_firsts("else_block"))
    {
        if_node->next_else = this->else_block();
    }
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::ifkw);
    return if_node;
};

ElifNode *Parser::elsif_stmt()
{
    /*
    elsif_stmt:
        | ELSIFKW expression THENKW block elsif_stmt
        | ELSIFKW expression THENKW block else_block
        | ELSIFKW expression THENKW block
     */
    this->check_get_next(Type::elsifkw);
    ExpressionNode *condition = this->expression();
    this->check_get_next(Type::thenkw);
    BlockNode *body = this->block();
    ElifNode *elif_node = new ElifNode(condition, body);
    if (this->is_token_in_firsts("elsif_stmt"))
    {
        elif_node->next_elif = this->elsif_stmt();
    }
    else if (this->is_token_in_firsts("else_block"))
    {
        elif_node->next_else = this->else_block();
    }
    return elif_node;
};

ElseNode *Parser::else_block()
{
    /*
    else_block:
        | ELSEKW block
     */
    this->check_get_next(Type::elsekw);
    BlockNode *body = this->block();
    return new ElseNode(body);
};

WhileNode *Parser::while_stmt()
{
    /*
    while_stmt:
        | WHILEKW expression LOOPKW block ENDKW LOOPKW
     */
    this->check_get_next(Type::whilekw);
    ExpressionNode *condition = this->expression();
    this->check_get_next(Type::loopkw);
    BlockNode *body = this->block();
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::loopkw);
    return new WhileNode(condition, body);
};

ForNode *Parser::for_stmt()
{
    /*
    for_stmt:
        | FORKW ID IN (NUMBER | ID) DOUBLEDOT (NUMBER | ID) LOOPKW block ENDKW LOOPKW
     */
    this->check_get_next(Type::forkw);
    Leaf *iterator = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::in);
    Leaf *from;
    Leaf *to;
    if (this->token_matches_any({Type::number, Type::id}))
    {
        from = new Leaf(this->get_token());
        this->next_token();
    }
    else
    {
        this->error("for_stmt");
    }
    this->check_get_next(Type::doubledot);
    if (this->token_matches_any({Type::number, Type::id}))
    {
        to = new Leaf(this->get_token());
        this->next_token();
    }
    else
    {
        this->error("for_stmt");
    }
    this->check_get_next(Type::loopkw);
    BlockNode *body = this->block();
    this->check_get_next(Type::endkw);
    this->check_get_next(Type::loopkw);
    return new ForNode(iterator, from, to, body);
};

void Parser::simple_stmt(BlockNode *parent_block)
{
    /*
    simple_stmt:
        | assignment
        | expression
        | return_stmt
     */
    if (this->is_token_in_firsts("assignment") && this->forward(1).getType() == Type::assign)
    {
        parent_block->add_child(this->assignment());
    }
    else if (this->is_token_in_firsts("expression"))
    {
        parent_block->add_child(this->expression());
    }
    else if (this->is_token_in_firsts("return_stmt"))
    {
        parent_block->add_child(this->return_stmt());
    }
    else
    {
        this->error("simple_stmt");
    }
};

AssignmentNode *Parser::assignment()
{
    /*
    assignment:
        | ID ASSIGN expression
     */
    Leaf *left = new Leaf(this->check_get_next(Type::id));
    this->check_get_next(Type::assign);
    ExpressionNode *right = this->expression();
    return new AssignmentNode(left, right);
};

ReturnNode *Parser::return_stmt()
{
    /*
    return_stmt:
        | RETURNKW expression
     */
    this->check_get_next(Type::returnkw);
    return new ReturnNode(this->expression());
};

ExpressionNode *Parser::expression()
{
    /*
    expression:
        | disjunction
     */
    return this->disjunction();
};

ExpressionNode *Parser::disjunction()
{
    /*
    disjunction:
        | conjunction OR disjunction
        | conjunction
     */
    ExpressionNode *left = this->conjunction();
    if (token_matches(Type::orop))
    {
        Token op = this->check_get_next(Type::orop);
        ExpressionNode *right = this->conjunction();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (token_matches(Type::orop))
        {
            op = this->check_get_next(Type::orop);
            right = this->conjunction();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
};

ExpressionNode *Parser::conjunction()
{
    /*
    conjunction:
        | inversion AND conjunction
        | inversion
     */
    ExpressionNode *left = this->inversion();
    if (token_matches(Type::andop))
    {
        Token op = this->check_get_next(Type::andop);
        ExpressionNode *right = this->inversion();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (token_matches(Type::andop))
        {
            op = this->check_get_next(Type::andop);
            right = this->inversion();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
};

ExpressionNode *Parser::inversion()
{
    /*
    inversion:
        | NOT inversion
        | comparison
     */
    if (this->get_token().getType() == Type::notop)
    {
        Token op = this->check_get_next(Type::notop);
        ExpressionNode *operand = this->inversion();
        return new UnaryNode(new Leaf(op), operand);
    }
    else
    {
        return this->comparison();
    }
};

ExpressionNode *Parser::comparison()
{
    /*
    comparison:
        | sum GREATER comparison
        | sum LESS comparison
        | sum EQUAL comparison
        | sum NOTEQ comparison
        | sum GREQUAL comparison
        | sum LEQUAL comparison
        | sum
     */
    ExpressionNode *left = this->sum();
    std::vector<Type> ok_ops = {Type::greater, Type::less, Type::equal, Type::noteq, Type::grequal, Type::lequal};
    if (this->token_matches_any(ok_ops))
    {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->sum();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (this->token_matches_any(ok_ops))
        {
            op = this->get_token();
            this->next_token();
            right = this->sum();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
}

ExpressionNode *Parser::sum()
{
    /*
    sum:
        | term PLUS sum
        | term MINUS sum
        | term
     */
    ExpressionNode *left = this->term();
    std::vector<Type> ok_ops = {Type::plus, Type::minus};
    if (this->token_matches_any(ok_ops))
    {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->term();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (this->token_matches_any(ok_ops))
        {
            op = this->get_token();
            this->next_token();
            right = this->term();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
};

ExpressionNode *Parser::term()
{
    /*
    term:
        | factor STAR term
        | factor DIV term
        | factor MOD term
        | factor
     */
    ExpressionNode *left = this->factor();
    std::vector<Type> ok_ops = {Type::star, Type::div, Type::mod};
    if (this->token_matches_any(ok_ops))
    {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *right = this->factor();
        BinaryNode *op_node = new BinaryNode(left, new Leaf(op), right);
        while (this->token_matches_any(ok_ops))
        {
            op = this->get_token();
            this->next_token();
            right = this->factor();
            op_node = new BinaryNode(op_node, new Leaf(op), right);
        }
        return op_node;
    }
    else
    {
        return left;
    }
};

ExpressionNode *Parser::factor()
{
    /*
    factor:
        | PLUS factor
        | MINUS factor
        | primary
     */
    if (this->token_matches_any({Type::plus, Type::minus}))
    {
        Token op = this->get_token();
        this->next_token();
        ExpressionNode *operand = this->factor();
        return new UnaryNode(new Leaf(op), operand);
    }
    else
    {
        return this->primary();
    }
};

ExpressionNode *Parser::primary()
{
    /*
    primary:
        | LPR expression RPR
        | atom
        | atom func_call
     */
    if (this->token_matches(Type::lpr))
    {
        this->check_get_next(Type::lpr);
        auto expr = this->expression();
        this->check_get_next(Type::rpr);
        return expr;
    }
    else if (this->is_token_in_firsts("atom"))
    {
        Leaf *atom = this->atom();
        if (this->is_token_in_firsts("func_call"))
        {
            ActualParamsNode *params = this->func_call();
            return new CallNode(atom->token, params);
        }
        return atom;
    }
    this->error("primary");
    return nullptr;
};

ActualParamsNode *Parser::func_call()
{
    /*
    func_call:
        | LPR actual_params RPR
        | LPR RPR
     */
    this->check_get_next(Type::lpr);
    if (this->is_token_in_firsts("actual_params"))
    {
        ActualParamsNode *params = this->actual_params();
        this->check_get_next(Type::rpr);
        return params;
    }
    else
    {
        this->check_get_next(Type::rpr);
        return new ActualParamsNode({});
    }
};

Leaf *Parser::atom()
{
    /*
    atom:
        | ID
        | STRING
        | NUMBER
     */
    if (this->token_matches_any({Type::id, Type::string, Type::number}))
    {
        Leaf *leaf = new Leaf(this->get_token());
        this->next_token();
        return leaf;
    }
    else
    {
        this->error("atom");
        return nullptr;
    }
};

ActualParamsNode *Parser::actual_params()
{
    /*
    arguments:
        | expression COMMA actual_params
        | expression
     */
    ActualParamsNode *params = new ActualParamsNode({});
    while (this->is_token_in_firsts("actual_params"))
    {
        params->add_child(this->expression());
        if (this->get_token().getType() == Type::comma)
        {
            this->next_token();
        }
    }
    return params;
}
std::vector<VariableDeclarationNode *> Parser::variable_declarations()
{
    /*
    variable_declarations:
        | variable_declaration SEMICOLON variable_declarations
        | variable_declaration SEMICOLON
    */
    std::vector<VariableDeclarationNode *> result = {};
    while (this->is_token_in_firsts("variable_declaration"))
    {
        result.push_back(this->variable_declaration());
        this->check_get_next(Type::semicolon);
    }
    return result;
};

VariableDeclarationNode *Parser::variable_declaration()
{
    /*
    variable_declaration:
        | ID COLON ID
        | ID COLON ARRAYKW LPR NUMBER DOUBLEDOT NUMBER RPR OFKW ID
    */
    Token id = this->check_get_next(Type::id);
    this->check_get_next(Type::colon);
    if (this->token_matches(Type::id))
    {
        Token type = this->check_get_next(Type::id);
        return new VariableDeclarationNode(id, new Leaf(type));
    }
    else
    {
        this->check_get_next(Type::arraykw);
        this->check_get_next(Type::lpr);
        this->check_get_next(Type::number);
        this->check_get_next(Type::doubledot);
        int size = std::stoi(this->check_get_next(Type::number).getValue());
        this->check_get_next(Type::rpr);
        this->check_get_next(Type::ofkw);
        Token type = this->check_get_next(Type::id);
        return new VariableDeclarationNode(id, new Leaf(type), size);
    }
}
