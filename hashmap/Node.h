#pragma once

template <typename T>
class Node {
    template <typename R> friend class HashMap;

    public:
        Node(int key, T value, Node<T> *next = nullptr);
    private:
        int key;
        T value;
        Node *next;
};

template <typename T>
Node<T>::Node(int key, T value, Node<T> *next)
    : key(key)
    , value(value)
    , next(next)
{

}
