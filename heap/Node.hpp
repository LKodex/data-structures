#pragma once

template<typename T>
class Node
{
    template<typename R> friend class Heap;
    public:
        Node(int priority, T element);
        ~Node();
    private:
        int priority;
        T element;
};

template<typename T>
Node<T>::Node(int priority, T element)
    : priority(priority)
    , element(element)
{
}

template<typename T>
Node<T>::~Node()
{
}
