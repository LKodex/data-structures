#include "Node.h"

class HashTable {
    public:
        HashTable(int size);
        ~HashTable();
        int remove(int key);
        void put(int key, int value);
        bool contains(int key);
        void clear();
    private:
        int size;
        Node **table;
        int hash(Node *node);
        int hash(int value);
};

HashTable::HashTable(int size)
    : size(size)
{
    table = new Node *[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = nullptr;
    }
}

HashTable::~HashTable()
{
    clear();
}

void HashTable::put(int key, int value)
{
    int index = hash(key);
    Node *node = new Node(key, value, table[index]);
    table[index] = node;
}

bool HashTable::contains(int value)
{
    int index = hash(key);
    while (table[index] != nullptr)
    {
        if (table[index]->value)
        {
            /* code */
        }
        
    }
    return false;
    
}

int HashTable::hash(Node *node)
{
    int key = node->key;
    return hash(key);
}

int HashTable::hash(int value)
{
    int hash = (value * 20131103) % size;
    return hash;
}