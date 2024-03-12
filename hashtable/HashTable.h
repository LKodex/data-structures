#pragma once

#include "HashTable.h"
#include "Node.h"
#include "iostream"
#include "iomanip"

template <typename T>
class HashTable {
    public:
        HashTable(int size);
        ~HashTable();
        T remove(int key);
        void put(int key, T value);
        bool contains(int key);
        void clear();
        void print();
        int elements();
        double loadFactor();
    private:
        int size;
        Node<T> **table;
        int hash(Node<T> *node);
        int hash(int value);
};

template <typename T>
HashTable<T>::HashTable(int size)
    : size(size)
{
    table = new Node<T> *[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = nullptr;
    }
}

template <typename T>
HashTable<T>::~HashTable()
{
    clear();
}

template <typename T>
T HashTable<T>::remove(int key)
{
    int index = hash(key);
    Node<T> *node = table[index];
    Node<T> **prev = &table[index];
    while (node != nullptr)
    {
        if (node->key == key)
        {
            T value = node->value;
            *prev = node->next;
            delete node;
            return value;
        }
        prev = &node;
        node = node->next;
    }
    return nullptr;
}

template <typename T>
void HashTable<T>::put(int key, T value)
{
    int index = hash(key);
    Node<T> *node = new Node<T>(key, value, table[index]);
    table[index] = node;
}

template <typename T>
bool HashTable<T>::contains(int key)
{
    int index = hash(key);
    Node<T> *node = table[index];
    while (node != nullptr)
    {
        if (node->key == key)
        {
            return true;
        }
        node = node->next;
    }
    return false;
}

template <typename T>
void HashTable<T>::clear()
{
    for (int i = 0; i < this->size; i++)
    {
        Node<T> *node = table[i];
        Node<T> *next = nullptr;
        while (node != nullptr)
        {
            next = node->next;
            delete node;
            node = next;
        }
    }
    delete table;
}

template <typename T>
void HashTable<T>::print()
{
    std::cout << "> Table size: " << size << std::endl;
    std::cout << "> Elements: " << elements() << std::endl;
    std::cout << "> Load factor: " << loadFactor() << std::endl;
    for (int i = 0; i < size; i++)
    {
        std::cout << std::setw(3) << i << ". ";
        Node<T> *node = table[i];
        while (node != nullptr)
        {
            std::cout << "K: " << std::setw(3) << node->key << " V: " << std::setw(15) << node->value;
            std::cout << " -> ";
            if (node->next == nullptr)
            {
                std::cout << " nullptr" << std::endl;
            }
            node = node->next;
        }
    }
}

template <typename T>
int HashTable<T>::elements()
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        Node<T> *node = table[i];
        while (node != nullptr)
        {
            count++;
            node = node->next;
        }
    }
    return count;
}

template <typename T>
double HashTable<T>::loadFactor()
{
    double quantity = (double) elements();
    double loadFactor = quantity / (double) size;
    return loadFactor;
}

template <typename T>
int HashTable<T>::hash(Node<T> *node)
{
    int key = node->key;
    return hash(key);
}

template <typename T>
int HashTable<T>::hash(int value)
{
    const int PRIME_NUMBER = 757;
    int hash = value * PRIME_NUMBER % size;
    return hash;
}
