/**============================================================================
Name        : asserts.hpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : asserts.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_ASSERTS_HPP
#define CPPFIRMWAREEMBEDDED_ASSERTS_HPP

#include <cstdlib>
#include <iostream>

#define ASSERT_TRUE(expression)                                                \
    do                                                                         \
    {                                                                          \
        if (!(expression))                                                     \
        {                                                                      \
            std::cerr                                                          \
                << "Assertion failed: " << #expression << '\n'                 \
                << "File: " << __FILE__ << '\n'                                \
                << "Line: " << __LINE__ << std::endl;                          \
            std::terminate();                                                  \
        }                                                                      \
    } while (false)

#define ASSERT_FALSE(expression) ASSERT_TRUE(!(expression))

#define ASSERT_EQ(lhs, rhs) ASSERT_TRUE((lhs) == (rhs))

#define ASSERT_NEAR(lhs, rhs, epsilon)                                         \
    ASSERT_TRUE(std::abs((lhs) - (rhs)) <= (epsilon))


#endif //CPPFIRMWAREEMBEDDED_ASSERTS_HPP
