#include <iostream>
#include "tries.h"

int main()
{
    TRIES::TRIE_p test;
    test.insert("toto");
    test.insert("abracadabra");
    test.insert("abra");
    test.insert("toa");

    std::cout << test.contains("abr") << std::endl;
    std::cout << test.contains("abracadabra") << std::endl;
    std::cout << test.prefix("anaconda") << std::endl;;
    return 0;
}
