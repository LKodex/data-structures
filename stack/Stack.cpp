#include "Stack.h"

Stack::Stack()
    : top(nullptr)
{

}

Stack::~Stack()
{
    clear();
}

void Stack::push(int value)
{
    Node *node = new Node(value);
    node->next = top;
    top = node;
}

int Stack::pop()
{
    if (isEmpty())
    {
        return 0;
    }
    Node *node = top;
    top = node->next;
    int value = node->value;
    delete node;
    return value;
}

void Stack::clear()
{
    while (!isEmpty())
    {
        pop();
    }
}

bool Stack::isEmpty()
{
    return top == nullptr;
}
