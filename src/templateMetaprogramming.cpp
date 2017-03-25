#include "templateMetaprogramming.h"

#include <iostream>

void templateMetaprogramming()
{
    std::cout << mp_size<std::tuple<int, char, float>>::value << std::endl;
    std::cout << mp_size_alt<std::tuple<char, float>>::value << std::endl;
}
