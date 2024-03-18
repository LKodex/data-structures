#pragma once

#include "Node.hpp"
#include <stdlib.h>

template<typename T>
class Heap
{
    public:
        Heap(int initialSize = 10, int growFactor = 10);
        ~Heap();
    private:
        int left(int i);
        int right(int i);
        int parent(int i);
        void down(int i);
        void up(int i);
        Node<T> *array;
        int maxSize;
        int size;
        int growFactor;
};

template<typename T>
Heap<T>::Heap(int initialSize, int growFactor)
    : size(0)
    , maxSize(initialSize)
    , growFactor(growFactor)
{
    array = calloc(initialSize, sizeof(Node<T>));
}

template<typename T>
Heap<T>::~Heap()
{
}

template<typename T>
void Heap<T>::up(int i)
{
    int parent = parent(i);
    while (array[parent].key > array[i].key)
    {
        Node<T> aux = array[parent]
        array[parent] = array[i]
        array[i] = aux;
        i = parent;
        parent = parent(i);
        if (parent < 0)
        {
            return;
        }
    }
}

template<typename T>
void Heap<T>::down(int i)
{
    int left = left(i);
    int right = right(i);
    int highest = i;
    if (right < size && array[right].key > array[highest].key)
    {
        highest = right;
    }
    if (left < size && array[left].key > array[highest].key)
    {
        highest = left;
    }
    if (highest != i)
    {
        Node<T> aux = array[highest];
        array[highest] = array[i];
        array[i] = aux;
        down(highest);
    }
}

template<typename T>
int Heap<T>::left(int i)
{
    return i * 2 + 1;
}

template<typename T>
int Heap<T>::right(int i)
{
    return i * 2 + 2;
}

template<typename T>
int Heap<T>::parent(int i)
{
    return (i - 1) / 2;
}