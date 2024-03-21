#include "HashMap.h"
#include <stdlib.h>
#include <string>
#include "iostream"

int main(int argc, char **argv)
{
    HashMap<std::string> hashmap(11);
    hashmap.put(1, "amendum");
    hashmap.put(2, "britski");
    hashmap.put(3, "caedum");
    hashmap.put(4, "daredo");
    hashmap.put(5, "edinis");
    hashmap.put(6, "farfel");
    hashmap.put(7, "galiz");
    hashmap.put(8, "huryan");
    hashmap.put(9, "indignum");
    hashmap.put(10, "jeenial");
    hashmap.put(11, "kaledro");
    hashmap.put(12, "lumaz");
    hashmap.put(13, "marcos");
    hashmap.put(14, "nicolas");
    hashmap.put(15, "osvaldo");
    hashmap.put(16, "pedro");
    hashmap.put(17, "queijo");
    hashmap.put(18, "raul");
    hashmap.put(19, "sucarado");
    hashmap.put(20, "tomas");
    hashmap.put(21, "urbano");
    hashmap.put(22, "victor");
    hashmap.put(23, "walter white");
    hashmap.put(24, "xamundao");
    hashmap.put(25, "yazuo");
    hashmap.put(26, "zighys");

    hashmap.print();
    std::string b = hashmap.remove(24);
    hashmap.print();

    return EXIT_SUCCESS;
}