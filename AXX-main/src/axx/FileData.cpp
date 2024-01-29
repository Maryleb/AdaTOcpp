#include <axx/lexer/FileData.hpp>

FileData::FileData(){}

Token FileData::get()
{
    Token tok = queue.front();
    queue.pop();
    return tok;
}

void FileData::put(Type _type, unsigned int _row, unsigned int _pos)
{
    queue.emplace(accum, _type, _row, _pos);
    accum.clear();
}
