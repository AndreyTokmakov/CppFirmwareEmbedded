/**============================================================================
Name        : main.cpp
Created on  : 
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description :
============================================================================**/

#include <iostream>
#include <string_view>
#include <vector>

#include "frequency.hpp"
#include "temperature.hpp"
#include "voltage.hpp"


namespace temperature_tests
{
    using namespace units;
    using namespace units::literals;

    void unitTests()
    {
        Temperature cpuTemperature = 72.5_degC;
        if (cpuTemperature > 85.0_degC) {
            std::cout << "emergency shutdown()\n";
        }

        cpuTemperature = 92.5_degC;
        if (cpuTemperature > 85.0_degC) {
            std::cout << "emergency shutdown()\n";
        }
    }

    void test2()
    {
        constexpr Temperature ambient = 25.0_degC;
        constexpr TemperatureDifference heating = TemperatureDifference::fromKelvin(40.0);
        constexpr Temperature cpu = ambient + heating;
    }

}

int main([[maybe_unused]] const int argc,
         [[maybe_unused]] char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    temperature_tests::unitTests();
    temperature_tests::test2();

    return EXIT_SUCCESS;
}

