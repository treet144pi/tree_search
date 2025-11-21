#include "runner.hpp"
#include <iostream>
#include <exception>

int main()
{
    try {
    return launcher(std::cin, std::cout,true);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
