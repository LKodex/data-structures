#include "HashTable.h"
#include <stdlib.h>
#include <string>
#include "iostream"

int main(int argc, char **argv)
{
    HashTable<std::string> hashtable(11);
    hashtable.put(1, "amendum");
    hashtable.put(2, "britski");
    hashtable.put(3, "caedum");
    hashtable.put(4, "daredo");
    hashtable.put(5, "edinis");
    hashtable.put(6, "farfel");
    hashtable.put(7, "galiz");
    hashtable.put(8, "huryan");
    hashtable.put(9, "indignum");
    hashtable.put(10, "jeenial");
    hashtable.put(11, "kaledro");
    hashtable.put(12, "lumaz");
    hashtable.put(13, "marcos");
    hashtable.put(14, "nicolas");
    hashtable.put(15, "osvaldo");
    hashtable.put(16, "pedro");
    hashtable.put(17, "queijo");
    hashtable.put(18, "raul");
    hashtable.put(19, "sucarado");
    hashtable.put(20, "tomas");
    hashtable.put(21, "urbano");
    hashtable.put(22, "victor");
    hashtable.put(23, "walter white");
    hashtable.put(24, "xamundao");
    hashtable.put(25, "yazuo");
    hashtable.put(26, "zighys");

    hashtable.print();
    std::string b = hashtable.remove(24);
    hashtable.print();

    return EXIT_SUCCESS;
}