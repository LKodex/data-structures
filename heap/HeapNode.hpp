#pragma once

template<typename T>
class HeapNode
{
    template<typename R> friend class Heap;
    public:
        HeapNode(int priority, T element);
        ~HeapNode();
    private:
        int priority;
        T element;
};

template<typename T>
HeapNode<T>::HeapNode(int priority, T element)
    : priority(priority)
    , element(element)
{
}

template<typename T>
HeapNode<T>::~HeapNode()
{
}
