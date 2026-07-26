/**============================================================================
Name        : temperature_test.cpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : temperature_test.cpp
============================================================================**/

#include <iostream>
#include "asserts.hpp"
#include "temperature.hpp"

using namespace units;
using namespace units::literals;

namespace
{
    void testTemperatureDifferenceDefaultConstructor()
    {
        constexpr TemperatureDifference difference;

        ASSERT_EQ(difference.kelvin(), 0.0);
        ASSERT_TRUE(difference.isZero());
    }

    void testTemperatureDifferenceFactoryMethods()
    {
        ASSERT_EQ(TemperatureDifference::fromKelvin(10.0).kelvin(), 10.0);
        ASSERT_EQ(TemperatureDifference::fromCelsius(20.0).kelvin(), 20.0);
        ASSERT_EQ(TemperatureDifference::fromFahrenheit(18.0).kelvin(), 10.0);
    }

    void testTemperatureDifferenceConversions()
    {
        constexpr auto difference = TemperatureDifference::fromKelvin(40.0);

        ASSERT_EQ(difference.kelvin(), 40.0);
        ASSERT_EQ(difference.celsius(), 40.0);
        ASSERT_EQ(difference.fahrenheit(), 72.0);
    }

    void testTemperatureDifferenceArithmetic()
    {
        constexpr auto first = 40.0_dK;
        constexpr auto second = 10.0_dK;
        constexpr auto sum = first + second;
        constexpr auto diff = first - second;

        ASSERT_EQ(sum.kelvin(), 50.0);
        ASSERT_EQ(diff.kelvin(), 30.0);
    }

    void testTemperatureDifferenceScalarOperations()
    {
        constexpr auto difference = 20.0_dK;

        ASSERT_EQ((difference * 2.0).kelvin(), 40.0);
        ASSERT_EQ((difference / 2.0).kelvin(), 10.0);
    }

    void testTemperatureDifferenceLiterals()
    {
        ASSERT_EQ((10.0_dK).kelvin(), 10.0);
        ASSERT_EQ((10.0_ddegC).kelvin(), 10.0);
        ASSERT_NEAR((18.0_ddegF).kelvin(), 10.0, 1e-12);
    }

    void testTemperatureDefaultConstructor()
    {
        constexpr Temperature temperature;

        ASSERT_EQ(temperature.kelvin(), 0.0);
        ASSERT_TRUE(temperature.isZero());
    }

    void testTemperatureFactoryMethods()
    {
        ASSERT_EQ(Temperature::fromKelvin(300.0).kelvin(), 300.0);
        ASSERT_EQ(Temperature::fromCelsius(0.0).kelvin(), 273.15);
        ASSERT_NEAR(Temperature::fromFahrenheit(32.0).kelvin(), 273.15, 1e-12);
    }

    void testTemperatureConversions()
    {
        constexpr auto temperature = 25.0_degC;

        ASSERT_NEAR(temperature.kelvin(), 298.15, 1e-12);
        ASSERT_NEAR(temperature.celsius(), 25.0, 1e-12);
        ASSERT_NEAR(temperature.fahrenheit(), 77.0, 1e-12);
    }

    void testTemperatureLiterals()
    {
        ASSERT_EQ((100.0_K).kelvin(), 100.0);
        ASSERT_NEAR((0.0_degC).kelvin(), 273.15, 1e-12);
        ASSERT_NEAR((32.0_degF).kelvin(), 273.15, 1e-12);
    }

    void testTemperatureComparisonOperators()
    {
        constexpr Temperature first = 20.0_degC;
        constexpr Temperature second = 30.0_degC;

        ASSERT_TRUE(first < second);
        ASSERT_TRUE(second > first);
        ASSERT_TRUE(first != second);
        ASSERT_TRUE(first == first);
    }

    void testTemperatureAddDifference()
    {
        constexpr Temperature ambient = 25.0_degC;
        constexpr TemperatureDifference rise = 40.0_dK;

        constexpr auto result = ambient + rise;

        ASSERT_NEAR(result.celsius(), 65.0, 1e-12);
    }

    void testTemperatureSubtractDifference()
    {
        constexpr Temperature temperature = 100.0_degC;
        constexpr TemperatureDifference decrease = 20.0_dK;
        constexpr auto result = temperature - decrease;

        ASSERT_NEAR(result.celsius(), 80.0, 1e-12);
    }

    void testTemperatureSubtractTemperature()
    {
        constexpr Temperature first = 100.0_degC;
        constexpr Temperature second = 40.0_degC;
        constexpr auto difference = first - second;

        ASSERT_NEAR(difference.kelvin(), 60.0, 1e-12);
    }

    void testTemperatureCompoundOperators()
    {
        auto temperature = 20.0_degC;

        temperature += 10.0_dK;
        ASSERT_NEAR(temperature.celsius(), 30.0, 1e-12);

        temperature -= 5.0_dK;
        ASSERT_NEAR(temperature.celsius(), 25.0, 1e-12);
    }

    void testTemperatureSign()
    {
        ASSERT_TRUE(Temperature::fromKelvin(10.0).isPositive());
        ASSERT_FALSE(Temperature::fromKelvin(-10.0).isPositive());
        ASSERT_TRUE(Temperature::fromKelvin(-10.0).isNegative());
    }

    constexpr bool testConstexprImpl()
    {
        constexpr Temperature ambient = 25.0_degC;
        constexpr TemperatureDifference rise = 40.0_dK;
        constexpr auto cpu = ambient + rise;
        return cpu.celsius() == 65.0;
    }

    void testConstexpr()
    {
        static_assert(testConstexprImpl());
        ASSERT_TRUE(testConstexprImpl());
    }
}


void temperature_test()
{
    testTemperatureDifferenceDefaultConstructor();
    testTemperatureDifferenceFactoryMethods();
    testTemperatureDifferenceConversions();
    testTemperatureDifferenceArithmetic();
    testTemperatureDifferenceScalarOperations();
    testTemperatureDifferenceLiterals();

    testTemperatureDefaultConstructor();
    testTemperatureFactoryMethods();
    testTemperatureConversions();
    testTemperatureLiterals();
    testTemperatureComparisonOperators();
    testTemperatureAddDifference();
    testTemperatureSubtractDifference();
    testTemperatureSubtractTemperature();
    testTemperatureCompoundOperators();
    testTemperatureSign();

    testConstexpr();

    std::cout << "All Temperature tests passed.\n";
}