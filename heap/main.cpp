#include "iostream"
#include "Heap.hpp"
#include <string>

int main(int argc, char *argv[])
{
    Heap<std::string> heap;

    heap.put(1, std::string("amendum"));
    heap.put(2, std::string("britski"));
    heap.put(3, std::string("caedum"));
    heap.put(4, std::string("daredo"));
    heap.put(5, std::string("edinis"));
    heap.put(6, std::string("farfel"));
    heap.put(7, std::string("galiz"));
    heap.put(8, std::string("huryan"));
    heap.put(9, std::string("indignum"));
    heap.put(10, std::string("jeenial"));
    heap.put(11, std::string("kaledro"));
    heap.put(12, std::string("lumaz"));
    heap.put(13, std::string("marcos"));
    heap.put(14, std::string("nicolas"));
    heap.put(15, std::string("osvaldo"));
    heap.put(16, std::string("pedro"));
    heap.put(17, std::string("queijo"));
    heap.put(18, std::string("raul"));
    heap.put(19, std::string("sucarado"));
    heap.put(20, std::string("tomas"));
    heap.put(21, std::string("urbano"));
    heap.put(22, std::string("victor"));
    heap.put(23, std::string("walter"));
    heap.put(24, std::string("xamundao"));
    heap.put(25, std::string("yazuo"));
    heap.put(26, std::string("zighys"));

    for (int i = 0; i < 26; i++)
    {
        std::cout << i << ": " << heap.pop() << std::endl;
    }

    return EXIT_SUCCESS;
}