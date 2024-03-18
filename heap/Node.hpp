#pragma once

template<typename T>
class Node
{
    template<typename R> friend class Heap;
    public:
        Node(int key, T element);
        ~Node();
    private:
        int key;
        T element;
};

template<typename T>
Node<T>::Node(int key, T element)
    : key(key)
    , element(element)
{
}

template<typename T>
Node<T>::~Node()
{
}
