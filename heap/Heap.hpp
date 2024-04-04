#pragma once

#include "Node.hpp"
#include <stdlib.h>
#include <vector>

template<typename T>
class Heap
{
    public:
        Heap();
        Heap(Node<T> array, int size);
        Heap(Node<T> *array, int size);
        ~Heap();
        T pop();
        void put(int priority, T element);
    private:
        std::vector<Node<T>> vector;
        int left(int index);
        int right(int index);
        int parent(int index);
        void swap(int firstIndex, int secondIndex);
        void down(int index);
        void up(int index);
        void heapify();
};

template<typename T>
Heap<T>::Heap()
    : vector(std::vector<Node<T>>())
{
}

template<typename T>
Heap<T>::Heap(Node<T> array, int size)
{
    Heap();
    for (int i = 0; i < size; i++)
    {
        Node<T> *node = new Node<T>(array[i]);
        vector.push_back(*node);
    }
    heapify();
}

template<typename T>
Heap<T>::Heap(Node<T> *array, int size)
    : vector(std::vector<Node<T>>(array, array + size))
{
    heapify();
}

template<typename T>
Heap<T>::~Heap()
{
}

template<typename T>
T Heap<T>::pop()
{
    Node<T> firstNode = vector.front();
    T element = firstNode.element;
    vector.front() = vector.back();
    vector.pop_back();
    const int FIRST = 0;
    down(FIRST);
    return element;
}

template<typename T>
void Heap<T>::put(int priority, T element)
{
    Node<T> *node = new Node<T>(priority, element);
    vector.push_back(*node);
    int lastIndex = vector.size() - 1;
    up(lastIndex);
}

template<typename T>
int Heap<T>::left(int index)
{
    return index * 2 + 1;
}

template<typename T>
int Heap<T>::right(int index)
{
    return index * 2 + 2;
}

template<typename T>
int Heap<T>::parent(int index)
{
    return (index - 1) / 2;
}

template<typename T>
void Heap<T>::swap(int firstIndex, int secondIndex)
{
    Node<T> aux = vector[firstIndex];
    vector[firstIndex] = vector[secondIndex];
    vector[secondIndex] = aux;
}

template<typename T>
void Heap<T>::up(int index)
{
    int parentIndex = parent(index);
    while (vector.at(parentIndex).priority < vector.at(index).priority)
    {
        swap(index, parentIndex);
        index = parentIndex;
        parentIndex = parent(index);
        if (parentIndex < 0)
        {
            return;
        }
    }
}

template<typename T>
void Heap<T>::down(int index)
{
    int leftIndex = left(index);
    int rightIndex = right(index);
    int highest = index;
    if (rightIndex < vector.size() && vector[rightIndex].priority > vector[highest].priority)
    {
        highest = rightIndex;
    }
    if (leftIndex < vector.size() && vector[leftIndex].priority > vector[highest].priority)
    {
        highest = leftIndex;
    }
    if (highest != index)
    {
        swap(highest, index);
        down(highest);
    }
}

template<typename T>
void Heap<T>::heapify()
{
    int halfIndex = vector.size() / 2;
    for (int i = halfIndex - 1; i >= 0; i--)
    {
        down(i);
    }
}
