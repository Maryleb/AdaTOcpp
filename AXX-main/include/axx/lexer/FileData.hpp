#pragma once
#include <stack>
#include <string>
#include <queue>
#include <axx/token/Token.hpp>

typedef std::queue<Token> tokenQueue_t;

struct FileData
{
    unsigned int pos = 1;
    unsigned int row = 1;
    std::string accum;
    tokenQueue_t queue;
    Token get();
    void put(Type _type, unsigned int _row = 0, unsigned int _pos = 0);
    FileData();
};
