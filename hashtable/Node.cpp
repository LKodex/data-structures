#pragma once

#include "Node.h"

Node::Node(int key, int value, Node *next = nullptr)
    : key(key)
    , value(value)
    , next(next)
{

}
