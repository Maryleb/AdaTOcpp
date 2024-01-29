#pragma once
#include <axx/interface/NodeVisitorInterface.hpp>

class VisitableNodeInterface
{
public:
    virtual void accept(NodeVisitorInterface *_visitor) = 0;
    virtual ~VisitableNodeInterface() = default;
};
