#pragma once

class Node {
    friend class Stack;
    
    private:
        Node(int value);
        int value;
        Node *next;
};