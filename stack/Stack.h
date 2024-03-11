#pragma once

#include "Node.h"

class Stack {
    public:
        Stack();
        ~Stack();
        void push(int value);
        int pop();
        void clear();
        bool isEmpty();
    private:
        Node *top;
};