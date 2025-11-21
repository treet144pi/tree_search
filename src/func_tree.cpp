#include "runner.hpp"
#include <iostream>


int main()
{
    try
    {
    return launcher(std::cin, std::cout,false);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
