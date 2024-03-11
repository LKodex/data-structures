#include "Stack.h"

Stack::Stack()
    : top(nullptr)
{

}

Stack::~Stack()
{
    this->clear();
}

void Stack::push(int value)
{
    Node *node = new Node(value);
    node->next = this->top;
    this->top = node;
}

int Stack::pop()
{
    if (this->isEmpty())
    {
        return 0;
    }
    Node *node = this->top;
    this->top = node->next;
    int value = node->value;
    delete node;
    return value;
}

void Stack::clear()
{
    while (!this->isEmpty())
    {
        this->pop();
    }
}

bool Stack::isEmpty()
{
    return this->top == nullptr;
}
