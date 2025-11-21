#include "runner_set.hpp"
#include <iostream>
#include <exception>

int main()
{
    try
    {
    return launcher_set(std::cin, std::cout,false);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
