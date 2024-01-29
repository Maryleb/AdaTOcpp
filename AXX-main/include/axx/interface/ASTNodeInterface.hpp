#pragma once

class ASTNodeInterface
{
public:
    virtual ~ASTNodeInterface() = default;
    virtual void print(int indent) = 0;
};
