#include <axx/lexer/Lexer.hpp>
#include <axx/lexer/LexerStates.hpp>
#include <unordered_map>
#include <iostream>
#define CHARCOUNT 1000

Type Lexer::recognize(const std::string &_id) const
{
    static const std::unordered_map<std::string, Type> map = {
        {"if", Type::ifkw},
        {"elsif", Type::elsifkw},
        {"else", Type::elsekw},
        {"for", Type::forkw},
        {"while", Type::whilekw},
        {"return", Type::returnkw},
        {"abort", Type::abortkw},
        {"abs", Type::abskw},
        {"abstract", Type::abstractkw},
        {"accept", Type::acceptkw},
        {"access", Type::accesskw},
        {"aliased", Type::aliasedkw},
        {"all", Type::allkw},
        {"array", Type::arraykw},
        {"at", Type::atkw},
        {"begin", Type::beginkw},
        {"body", Type::bodykw},
        {"case", Type::casekw},
        {"constant", Type::constantkw},
        {"declare", Type::declarekw},
        {"delay", Type::delaykw},
        {"delta", Type::deltakw},
        {"digits", Type::digitskw},
        {"do", Type::dokw},
        {"end", Type::endkw},
        {"entry", Type::entrykw},
        {"exception", Type::exceptionkw},
        {"exit", Type::exitkw},
        {"function", Type::functionkw},
        {"generic", Type::generickw},
        {"goto", Type::gotokw},
        {"interface", Type::interfacekw},
        {"limited", Type::limitedkw},
        {"loop", Type::loopkw},
        {"new", Type::newkw},
        {"null", Type::nullkw},
        {"of", Type::ofkw},
        {"others", Type::otherskw},
        {"out", Type::outkw},
        {"overrid", Type::overridkw},
        {"package", Type::packagekw},
        {"pragma", Type::pragmakw},
        {"private", Type::privatekw},
        {"procedure", Type::procedurekw},
        {"protected", Type::protectedkw},
        {"raise", Type::raisekw},
        {"range", Type::rangekw},
        {"record", Type::recordkw},
        {"rem", Type::remkw},
        {"renames", Type::renameskw},
        {"requeue", Type::requeuekw},
        {"reverse", Type::reversekw},
        {"select", Type::selectkw},
        {"separate", Type::separatekw},
        {"some", Type::somekw},
        {"subtype", Type::subtypekw},
        {"sync", Type::synckw},
        {"tagged", Type::taggedkw},
        {"task", Type::taskkw},
        {"terminate", Type::terminatekw},
        {"then", Type::thenkw},
        {"type", Type::typekw},
        {"until", Type::untilkw},
        {"use", Type::usekw},
        {"when", Type::whenkw},
        {"while", Type::whilekw},
        {"with", Type::withkw},
        {"mod", Type::mod},
        {"not", Type::notop},
        {"in", Type::in},
        {"is", Type::is},
        {"and", Type::andop},
        {"or", Type::orop},
        {"xor", Type::xorop}
};

    if (map.find(_id) != map.end())
        return map.at(_id);
    else
        return Type::id;
}

void Lexer::open(std::istream &_stream)
{
    filedata.reset(new FileData());
    currBuff.reset(new std::string(CHARCOUNT + 1, '\0'));
    otherBuff.reset(new std::string());

    this->stream = &_stream;

    this->stream->read(&(*currBuff)[0], CHARCOUNT);
    iter = currBuff->cbegin();
    setState(new Start(this, filedata.get()));
}

void Lexer::setState(LexerStateInterface *_state)
{
    this->state.reset(_state);
}

Token Lexer::getToken()
{
    while (filedata->queue.empty() && iter != currBuff->cend())
    {
        this->state->recognize(*iter++);
    }

    Token tok = filedata->get();

    if (tok.getType() == Type::eof)
    {
        if (iter == currBuff->cend())
        {
            otherBuff->assign(CHARCOUNT + 1, '\0');
            stream->read(&(*otherBuff)[0], CHARCOUNT);
            iter = otherBuff->cbegin();
            currBuff.swap(otherBuff);
            while (filedata->queue.empty() && iter != currBuff->cend())
            {
                this->state->recognize(*iter++);
            }
            tok = filedata->get();
            if (tok.getType() == Type::eof)
            {
                return tok;
            }
        }
        else
        {
            return tok;
        }
    }

    if (tok.getType() == Type::id)
    {
        tok.setType(recognize(tok.getValue()));
    }
    return tok;
}

void Lexer::print_all_tokens() {
    Token token("", Type::id);
    while ((token = this->getToken()).getType() != Type::eof) {
        std::cout << type_to_str(token.getType()) << " " << token.getValue() << "\n";
    }
    std::cout << type_to_str(token.getType()) << " " << token.getValue() << "\n";
}
