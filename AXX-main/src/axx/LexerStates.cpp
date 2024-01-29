#include <axx/lexer/LexerStates.hpp>
#include <axx/lexer/FileData.hpp>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <string>

#define impl(name)                                                                         \
    name::name(LexerInterface *lex, FileData *filedata) : BaseLexerState(lex, filedata) {} \
    bool name::recognize(char _c)
#define newstate(name) lexer->setState(new name(lexer, filedata))
#define fac(state) \
    static inline LexerStateInterface *state##Factory(LexerInterface *a, FileData *b) { return new state(a, b); }
#define tab(symbol, state)     \
    {                          \
        symbol, state##Factory \
    }

typedef std::function<LexerStateInterface *(LexerInterface *, FileData *)> stateFactory_t;

fac(String)
fac(Character)
fac(Ampersand)
fac(Colon)
fac(Semicolon)
fac(Dot)
fac(VerticalLine)
fac(Plus)
fac(Minus)
fac(Star)
fac(Div)
fac(Greater)
fac(Less)
fac(Equal)
fac(Lpr)
fac(Rpr)
fac(Comma)
fac(Skip)
fac(Newline)

static std::unordered_map<char, stateFactory_t> table = {
    tab('&', Ampersand),
    tab('+', Plus),
    tab('-', Minus),
    tab('*', Star),
    tab('/', Div),
    tab('|', VerticalLine),
    tab('<', Less),
    tab('>', Greater),
    tab('=', Equal),
    tab('.', Dot),
    tab(',', Comma),
    tab('(', Lpr),
    tab(')', Rpr),
    tab(':', Colon),
    tab(';', Semicolon),
    tab('"', String),
    tab('\'', Character),
    tab('\n', Newline)
};

static const std::unordered_set<char> symbols = {
    '+', '-', '*', '/', '&', '|', '<', '>', '=', '.', ',', '(', ')', ':', ';', '\'', '"', '\n'};

static inline bool isSuitableForIdBeginning(char _c)
{
    return std::isalpha(_c);
}

static inline bool isSuitableForId(char _c)
{
    return std::isalnum(_c) || _c == '_';
}

BaseLexerState::BaseLexerState(LexerInterface *_lex, FileData *_filedata) : lexer(_lex), filedata(_filedata), initpos(_filedata->pos) {}

static stateFactory_t tablestate(char _c)
{
    if (symbols.find(_c) != symbols.end())
        return table[_c];
    else if (_c == '\0')
    {
        return nullptr;
    }
    else
    {
        return SkipFactory;
    }
}

impl(Start)
{
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else if (_c != '\n')
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Skip)
{
    filedata->pos++;
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Id)
{
    filedata->pos++;
    if (isSuitableForId(_c))
    {
        if (_c == '_' && hasUnderscore)
        {
            filedata->put(Type::unexpected, filedata->row, filedata->pos);
            return false;
        }
        else if (_c == '_' && !hasUnderscore)
        {
            hasUnderscore = true;
        }
        else
        {
            hasUnderscore = false;
        }
        filedata->accum.push_back(_c);
    }
    else
    {
        filedata->put(Type::id, filedata->row, initpos);
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(String)
{
    filedata->pos++;
    if (_c == '"')
    {
        filedata->put(Type::string, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->accum.push_back(_c);
    }
    return false;
}

impl(Character)
{
    filedata->pos++;
    filedata->accum.push_back(_c);
    if (_c == '\'')
    {
        filedata->put(Type::character, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->accum.push_back(_c);
    }
    return false;
}

impl(Colon)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::assign, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::colon, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Semicolon)
{
    filedata->pos++;
    filedata->put(Type::semicolon, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Ampersand)
{
    filedata->pos++;
    filedata->put(Type::ampersand, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(VerticalLine)
{
    filedata->pos++;
    filedata->put(Type::vertical, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Dot)
{
    filedata->pos++;
    if (_c == '.')
    {
        filedata->put(Type::doubledot, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::dot, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(FirstNumPart)
{
    filedata->pos++;
    if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
    }
    else if (_c == '.')
    {
        lexer->setState(new SecondNumPart(lexer, filedata, initpos));
    }
    else
    {
        filedata->put(Type::number, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

SecondNumPart::SecondNumPart(LexerInterface *_lex, FileData *_filedata, unsigned int _initpos) : BaseLexerState(_lex, _filedata)
{
    initpos = _initpos;
}
bool SecondNumPart::recognize(char _c)
{
    filedata->pos++;
    if (std::isdigit(_c))
    {
        if (created)
        {
            filedata->accum.push_back('.');
            created = false;
        }
        filedata->accum.push_back(_c);
    }
    else
    {
        filedata->put(Type::number, filedata->row, initpos);
        filedata->put(Type::dot, filedata->row, filedata->pos);
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Plus)
{
    filedata->pos++;
    filedata->put(Type::plus, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Minus)
{
    filedata->pos++;
    if (_c == '-')
    {
        newstate(Comment);
    }
    else
    {
        filedata->put(Type::minus, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Star)
{
    filedata->pos++;
    if (_c == '*')
    {
        filedata->put(Type::power, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::star, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Div)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::noteq, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::div, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Greater)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::grequal, filedata->row, initpos);
        newstate(Skip);
    }
    else if (_c == '>')
    {
        filedata->put(Type::rlabbr, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::greater, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Less)
{
    filedata->pos++;
    if (_c == '=')
    {
        filedata->put(Type::lequal, filedata->row, initpos);
        newstate(Skip);
    }
    else if (_c == '<')
    {
        filedata->put(Type::llabbr, filedata->row, initpos);
        newstate(Skip);
    }
    else if (_c == '>')
    {
        filedata->put(Type::box, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::less, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Equal)
{
    filedata->pos++;
    if (_c == '>')
    {
        filedata->put(Type::arrow, filedata->row, initpos);
        newstate(Skip);
    }
    else
    {
        filedata->put(Type::equal, filedata->row, initpos);
        if (isSuitableForIdBeginning(_c))
        {
            filedata->accum.push_back(_c);
            newstate(Id);
        }
        else if (std::isdigit(_c))
        {
            filedata->accum.push_back(_c);
            newstate(FirstNumPart);
        }
        else
        {
            auto p = tablestate(_c);
            ;
            if (p)
            {
                lexer->setState(p(lexer, filedata));
            }
            else
            {
                filedata->put(Type::eof, filedata->row, initpos);
            }
        }
    }
    return false;
}

impl(Lpr)
{
    filedata->pos++;
    filedata->put(Type::lpr, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Rpr)
{
    filedata->pos++;
    filedata->put(Type::rpr, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Comment)
{
    if (_c == '\n')
    {
        newstate(Newline);
    }
    return false;
}

impl(Comma)
{
    filedata->pos++;
    filedata->put(Type::comma, filedata->row, initpos);
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}

impl(Newline)
{
    filedata->pos = 1;
    filedata->row++;
    if (isSuitableForIdBeginning(_c))
    {
        filedata->accum.push_back(_c);
        newstate(Id);
    }
    else if (std::isdigit(_c))
    {
        filedata->accum.push_back(_c);
        newstate(FirstNumPart);
    }
    else
    {
        auto p = tablestate(_c);
        ;
        if (p)
        {
            lexer->setState(p(lexer, filedata));
        }
        else
        {
            filedata->put(Type::eof, filedata->row, initpos);
        }
    }
    return false;
}
