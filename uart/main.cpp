/**============================================================================
Name        : main.cpp
Created on  : 
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : C++ Experiments
============================================================================**/

#include <iostream>
#include <string_view>
#include <vector>

#include "uart.hpp"


int main([[maybe_unused]] const int argc,
         [[maybe_unused]] char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    // uart::experiments::testAll();
    uart::serial_port_reader::testAll();

    return EXIT_SUCCESS;
}

