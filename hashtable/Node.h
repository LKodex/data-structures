class Node {
    friend class HashTable;

    public:
        Node(int key, int value, Node *next);
    private:
        int key;
        int value;
        Node *next;
};
