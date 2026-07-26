
#include <iostream>

#include "asserts.hpp"
#include <voltage.hpp>

using namespace units;
using namespace units::literals;


namespace
{
    void testDefaultConstructor()
    {
        constexpr Voltage voltage;

        ASSERT_EQ(voltage.volts(), 0.0);
        ASSERT_TRUE(voltage.isZero());
    }

    void testFactoryMethods()
    {
        ASSERT_EQ(Voltage::fromVolts(123.0).volts(), 123.0);
        ASSERT_EQ(Voltage::fromMilliVolts(2.0).volts(), 0.002);
        ASSERT_EQ(Voltage::fromMicroVolts(3.0).volts(), 0.000003);
        ASSERT_EQ(Voltage::fromNanoVolts(4.0).volts(), 0.000000004);
        ASSERT_EQ(Voltage::fromKiloVolts(5.0).volts(), 5000.0);
    }

    void testUnitConversions()
    {
        constexpr auto voltage = Voltage::fromKiloVolts(2.5);

        ASSERT_EQ(voltage.kiloVolts(), 2.5);
        ASSERT_EQ(voltage.volts(), 2500.0);
        ASSERT_EQ(voltage.milliVolts(), 2'500'000.0);
        ASSERT_EQ(voltage.microVolts(), 2'500'000'000.0);
        ASSERT_EQ(voltage.nanoVolts(), 2'500'000'000'000.0);
    }

    void testComparisonOperators()
    {
        constexpr auto first = Voltage::fromVolts(3.3);
        constexpr auto second = Voltage::fromVolts(5.0);

        ASSERT_TRUE(first < second);
        ASSERT_TRUE(second > first);
        ASSERT_TRUE(first != second);
        ASSERT_TRUE(first == first);
    }

    void testAddition()
    {
        constexpr auto first = Voltage::fromVolts(3.3);
        constexpr auto second = Voltage::fromVolts(1.7);
        constexpr auto result = first + second;

        ASSERT_EQ(result.volts(), 5.0);
    }

    void testSubtraction()
    {
        constexpr auto first = Voltage::fromVolts(5.0);
        constexpr auto second = Voltage::fromVolts(3.3);
        constexpr auto result = first - second;

        ASSERT_NEAR(result.volts(), 1.7, 1e-12);
    }

    void testScalarMultiplication()
    {
        constexpr auto voltage = Voltage::fromVolts(2.5);

        ASSERT_EQ((voltage * 2.0).volts(), 5.0);
        ASSERT_EQ((2.0 * voltage).volts(), 5.0);
    }

    void testScalarDivision()
    {
        constexpr auto voltage = Voltage::fromVolts(5.0);

        ASSERT_EQ((voltage / 2.0).volts(), 2.5);
    }

    void testRatio()
    {
        constexpr auto first = Voltage::fromVolts(10.0);
        constexpr auto second = Voltage::fromVolts(5.0);

        ASSERT_EQ(first / second, 2.0);
    }

    void testCompoundOperators()
    {
        auto voltage = Voltage::fromVolts(3.0);

        voltage += Voltage::fromVolts(2.0);
        ASSERT_EQ(voltage.volts(), 5.0);

        voltage -= Voltage::fromVolts(1.0);
        ASSERT_EQ(voltage.volts(), 4.0);

        voltage *= 2.0;
        ASSERT_EQ(voltage.volts(), 8.0);

        voltage /= 4.0;
        ASSERT_EQ(voltage.volts(), 2.0);
    }

    void testZero()
    {
        ASSERT_TRUE(Voltage().isZero());
        ASSERT_TRUE(Voltage::fromVolts(0.0).isZero());
        ASSERT_FALSE(Voltage::fromVolts(1.0).isZero());
    }

    void testPositive()
    {
        ASSERT_TRUE(Voltage::fromVolts(1.0).isPositive());
        ASSERT_FALSE(Voltage::fromVolts(-1.0).isPositive());
    }

    void testNegative()
    {
        ASSERT_TRUE(Voltage::fromVolts(-1.0).isNegative());
        ASSERT_FALSE(Voltage::fromVolts(1.0).isNegative());
    }

    void testUserDefinedLiterals()
    {
        ASSERT_EQ((10_nV).nanoVolts(), 10.0);
        ASSERT_EQ((15_uV).microVolts(), 15.0);
        ASSERT_EQ((3300_mV).volts(), 3.3);
        ASSERT_EQ((12_V).volts(), 12.0);
        ASSERT_EQ((2_kV).volts(), 2000.0);
    }

    constexpr bool testConstexprImpl()
    {
        constexpr auto voltage = 3300_mV;
        return voltage.volts() == 3.3;
    }

    void testConstexpr()
    {
        //static_assert(testConstexprImpl());
        ASSERT_TRUE(testConstexprImpl());
    }
}

void voltage_test()
{
    testDefaultConstructor();
    testFactoryMethods();
    testUnitConversions();
    testComparisonOperators();
    testAddition();
    testSubtraction();
    testScalarMultiplication();
    testScalarDivision();
    testRatio();
    testCompoundOperators();
    testZero();
    testPositive();
    testNegative();
    testUserDefinedLiterals();
    testConstexpr();

    std::cout << "All Voltage tests passed.\n";
}