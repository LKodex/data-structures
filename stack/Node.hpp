#pragma once

class Node {
    friend class Stack;
    
    private:
        Node(int value);
        int value;
        Node *next;
};

Node::Node(int value)
    : value(value)
    , next(nullptr)
{

}
