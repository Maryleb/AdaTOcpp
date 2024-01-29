#pragma once
#include <string>

/// @brief Тип токена
enum class Type
{
    id,         // Идентификатор
    number,     // Любое число, на последующих этапах трансляции будет распознано как целое или вещественное
    string,     // Строка
    character,  // Символ
    lpr,        // Символ (
    rpr,        // Символ )
    colon,      // Символ :
    semicolon,  // Символ ;
    ampersand,  // Символ &
    box,        // Оператор <>
    llabbr,     // Оператор <<
    rlabbr,     // Оператор >>
    plus,       // Оператор +
    minus,      // Оператор -
    star,       // Оператор *
    power,      // Оператор **
    div,        // Оператор /
    dot,        // Оператор .
    doubledot,  // Оператор ..
    greater,    // Оператор >
    less,       // Оператор <
    equal,      // Оператор =
    noteq,      // Оператор /=
    grequal,    // Оператор >=
    lequal,     // Оператор <=
    mod,        // Оператор mod
    notop,      // Оператор not
    in,         // Оператор in
    is,         // Оператор is
    andop,      // Оператор and
    orop,       // Оператор or
    xorop,      // Оператор xor
    vertical,   // Оператор |
    assign,     // Оператор :=
    arrow,      // Оператор =>
    abortkw,    // Ключевое слово abort 
    abskw,      // Ключевое слово abs 
    abstractkw, // Ключевое слово abstract 
    acceptkw,   // Ключевое слово accept 
    accesskw,   // Ключевое слово access 
    aliasedkw,  // Ключевое слово aliased 
    allkw,      // Ключевое слово all 
    arraykw,    // Ключевое слово array 
    atkw,       // Ключевое слово at 
    beginkw,    // Ключевое слово begin 
    bodykw,     // Ключевое слово body 
    casekw,     // Ключевое слово case 
    constantkw, // Ключевое слово constant 
    declarekw,  // Ключевое слово declare 
    delaykw,    // Ключевое слово delay 
    deltakw,    // Ключевое слово delta 
    digitskw,   // Ключевое слово digits 
    dokw,       // Ключевое слово do 
    endkw,      // Ключевое слово end 
    entrykw,    // Ключевое слово entry 
    exceptionkw,// Ключевое слово exception 
    exitkw,     // Ключевое слово exit 
    functionkw, // Ключевое слово function 
    generickw,  // Ключевое слово generic 
    gotokw,     // Ключевое слово goto 
    interfacekw,// Ключевое слово interface 
    limitedkw,  // Ключевое слово limited 
    loopkw,     // Ключевое слово loop 
    newkw,      // Ключевое слово new 
    nullkw,     // Ключевое слово null 
    ofkw,       // Ключевое слово of 
    otherskw,   // Ключевое слово others 
    outkw,      // Ключевое слово out 
    overridkw,  // Ключевое слово overrid 
    packagekw,  // Ключевое слово package 
    pragmakw,   // Ключевое слово pragma 
    privatekw,  // Ключевое слово private 
    procedurekw,// Ключевое слово procedure 
    protectedkw,// Ключевое слово protected 
    raisekw,    // Ключевое слово raise 
    rangekw,    // Ключевое слово range 
    recordkw,   // Ключевое слово record 
    remkw,      // Ключевое слово rem 
    renameskw,  // Ключевое слово renames 
    requeuekw,  // Ключевое слово requeue 
    reversekw,  // Ключевое слово reverse 
    selectkw,   // Ключевое слово select 
    separatekw, // Ключевое слово separate 
    somekw,     // Ключевое слово some 
    subtypekw,  // Ключевое слово subtype 
    synckw,     // Ключевое слово sync 
    taggedkw,   // Ключевое слово tagged 
    taskkw,     // Ключевое слово task 
    terminatekw,// Ключевое слово terminate 
    thenkw,     // Ключевое слово then 
    typekw,     // Ключевое слово type 
    untilkw,    // Ключевое слово until 
    usekw,      // Ключевое слово use 
    whenkw,     // Ключевое слово when 
    withkw,     // Ключевое слово with 
    ifkw,       // Ключевое слово if
    elsifkw,    // Ключевое слово elsif
    elsekw,     // Ключевое слово else
    forkw,      // Ключевое слово for
    whilekw,    // Ключевое слово while
    returnkw,   // Ключевое слово return
    eof,        // Конец файла
    comma,      // Запятая
    unexpected, // Без комментариев
};

std::string type_to_str(Type type);

class Token
{
private:
    std::string value;
    Type type;
    unsigned int row;
    unsigned int pos;

public:
    std::string getValue() const;
    Type getType() const;
    unsigned int getPos() const;
    unsigned int getRow() const;
    void setValue(std::string _value);
    void setType(Type _type);
    Token(std::string _value, Type _type, unsigned int _row = 0, unsigned int _pos = 0);
    bool operator==(const Token& _other) const;
};
