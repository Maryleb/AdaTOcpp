#include <axx/token/Token.hpp>
#include <unordered_set>

std::string Token::getValue() const
{
    return value;
}

Type Token::getType() const
{
    return type;
}

unsigned int Token::getPos() const
{
    return pos;
}

unsigned int Token::getRow() const
{
    return row;
}

void Token::setValue(std::string _value)
{
    this->value = _value;
}

void Token::setType(Type _type)
{
    this->type = _type;
}

Token::Token(std::string _value, Type _type, unsigned int _row, unsigned int _pos) : value(_value), type(_type), row(_row), pos(_pos) {}

bool Token::operator==(const Token& _other) const
{
    return this->type == _other.getType() && this->value == _other.getValue();
}

std::string type_to_str(Type type) {
    switch (type) {
        case Type::id:
            return "id";
        case Type::number:
            return "number";
        case Type::string:
            return "string";
        case Type::character:
            return "character";
        case Type::lpr:
            return "lpr";
        case Type::rpr:
            return "rpr";
        case Type::colon:
            return "colon";
        case Type::semicolon:
            return "semicolon";
        case Type::ampersand:
            return "ampersand";
        case Type::box:
            return "box";
        case Type::llabbr:
            return "llabbr";
        case Type::rlabbr:
            return "rlabbr";
        case Type::plus:
            return "plus";
        case Type::minus:
            return "minus";
        case Type::star:
            return "star";
        case Type::power:
            return "power";
        case Type::div:
            return "div";
        case Type::dot:
            return "dot";
        case Type::doubledot:
            return "doubledot";
        case Type::greater:
            return "greater";
        case Type::less:
            return "less";
        case Type::equal:
            return "equal";
        case Type::noteq:
            return "noteq";
        case Type::grequal:
            return "grequal";
        case Type::lequal:
            return "lequal";
        case Type::mod:
            return "mod";
        case Type::notop:
            return "notop";
        case Type::in:
            return "in";
        case Type::is:
            return "is";
        case Type::andop:
            return "andop";
        case Type::orop:
            return "orop";
        case Type::xorop:
            return "xorop";
        case Type::vertical:
            return "vertical";
        case Type::assign:
            return "assign";
        case Type::arrow:
            return "arrow";
        case Type::abortkw:
            return "abortkw";
        case Type::abskw:
            return "abskw";
        case Type::abstractkw:
            return "abstractkw";
        case Type::acceptkw:
            return "acceptkw";
        case Type::accesskw:
            return "accesskw";
        case Type::aliasedkw:
            return "aliasedkw";
        case Type::allkw:
            return "allkw";
        case Type::arraykw:
            return "arraykw";
        case Type::atkw:
            return "atkw";
        case Type::beginkw:
            return "beginkw";
        case Type::bodykw:
            return "bodykw";
        case Type::casekw:
            return "casekw";
        case Type::constantkw:
            return "constantkw";
        case Type::declarekw:
            return "declarekw";
        case Type::delaykw:
            return "delaykw";
        case Type::deltakw:
            return "deltakw";
        case Type::digitskw:
            return "digitskw";
        case Type::dokw:
            return "dokw";
        case Type::endkw:
            return "endkw";
        case Type::entrykw:
            return "entrykw";
        case Type::exceptionkw:
            return "exceptionkw";
        case Type::exitkw:
            return "exitkw";
        case Type::functionkw:
            return "functionkw";
        case Type::generickw:
            return "generickw";
        case Type::gotokw:
            return "gotokw";
        case Type::interfacekw:
            return "interfacekw";
        case Type::limitedkw:
            return "limitedkw";
        case Type::loopkw:
            return "loopkw";
        case Type::newkw:
            return "newkw";
        case Type::nullkw:
            return "nullkw";
        case Type::ofkw:
            return "ofkw";
        case Type::otherskw:
            return "otherskw";
        case Type::outkw:
            return "outkw";
        case Type::overridkw:
            return "overridkw";
        case Type::packagekw:
            return "packagekw";
        case Type::pragmakw:
            return "pragmakw";
        case Type::privatekw:
            return "privatekw";
        case Type::procedurekw:
            return "procedurekw";
        case Type::protectedkw:
            return "protectedkw";
        case Type::raisekw:
            return "raisekw";
        case Type::rangekw:
            return "rangekw";
        case Type::recordkw:
            return "recordkw";
        case Type::remkw:
            return "remkw";
        case Type::renameskw:
            return "renameskw";
        case Type::requeuekw:
            return "requeuekw";
        case Type::reversekw:
            return "reversekw";
        case Type::selectkw:
            return "selectkw";
        case Type::separatekw:
            return "separatekw";
        case Type::somekw:
            return "somekw";
        case Type::subtypekw:
            return "subtypekw";
        case Type::synckw:
            return "synckw";
        case Type::taggedkw:
            return "taggedkw";
        case Type::taskkw:
            return "taskkw";
        case Type::terminatekw:
            return "terminatekw";
        case Type::thenkw:
            return "thenkw";
        case Type::typekw:
            return "typekw";
        case Type::untilkw:
            return "untilkw";
        case Type::usekw:
            return "usekw";
        case Type::whenkw:
            return "whenkw";
        case Type::withkw:
            return "withkw";
        case Type::ifkw:
            return "ifkw";
        case Type::elsifkw:
            return "elsifkw";
        case Type::elsekw:
            return "elsekw";
        case Type::forkw:
            return "forkw";
        case Type::whilekw:
            return "whilekw";
        case Type::returnkw:
            return "returnkw";
        case Type::eof:
            return "eof";
        case Type::comma:
            return "comma";
        case Type::unexpected:
            return "unexpected";
        default:
            return "";
    }
}