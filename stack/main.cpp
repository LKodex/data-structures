#include "iostream"
#include <stdlib.h>
#include "Stack.hpp"

int main(int argc, char **argv)
{
    Stack stack = Stack();
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.push(5);
    
    for (int i = 1; !stack.isEmpty(); i++)
    {
        std::cout << stack.pop() << std::endl;
    }

    return EXIT_SUCCESS;
}
