/**============================================================================
Name        : main.cpp
Created on  : 
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : C++ Experiments
============================================================================**/


#include <iostream>
#include <print>
#include <format>
#include <string_view>
#include <array>
#include <vector>
#include <thread>
#include <algorithm>
#include <numeric>
#include <array>
#include <cstdint>
#include <charconv>
#include <contracts>
#include <functional>
#include <typeindex>
#include <concepts>
#include <utility>
#include <variant>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>


namespace
{
    constexpr std::array<uint8_t, 27> bytes {
        0x31, 0x39, 0x37, 0x30, 0x2d, 0x30, 0x31, 0x2d, 0x30, 0x31, 0x20, 0x30, 0x30,
        0x3a, 0x32, 0x38, 0x3a, 0x34, 0x38, 0x2e, 0x32, 0x33, 0x35, 0x31, 0x38, 0x32, 0x0d
    };

    void parse1(const std::span<const uint8_t> strBytes)
    {
        const std::string_view str(reinterpret_cast<const char*>(strBytes.data()), strBytes.size());
        std::chrono::sys_time<std::chrono::microseconds> time;

        std::istringstream iss(str);
        iss >> std::chrono::parse("%Y-%m-%d %H:%M:%OS", time);
        if (iss.fail()){
            std::cout << "Parse error\n";
            return;
        }
        std::cout << std::chrono::floor<std::chrono::seconds>(time) << '\n';
    }

    void parse()
    {
        parse1(bytes);
    }
}

int main([[maybe_unused]] const int argc,
         [[maybe_unused]] char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);


    parse();

    return EXIT_SUCCESS;
}

