/**============================================================================
Name        : main.cpp
Created on  : 
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : C++ Firmware Tools
============================================================================**/

#include <string_view>
#include <vector>

#include "tools.hpp"

int main([[maybe_unused]] const int argc,
         [[maybe_unused]] char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    using namespace tools;

    ring_buffer_segmented::testAll();

    return EXIT_SUCCESS;
}

