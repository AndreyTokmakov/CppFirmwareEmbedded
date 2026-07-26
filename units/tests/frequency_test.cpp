#include <cmath>
#include <cstdlib>
#include <iostream>

#include <frequency.hpp>

using namespace units;
using namespace units::literals;


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


namespace
{
    void testDefaultConstructor()
    {
        constexpr Frequency frequency;

        ASSERT_EQ(frequency.hz(), 0.0);
        ASSERT_TRUE(frequency.isZero());
    }

    void testFactoryMethods()
    {
        ASSERT_EQ(Frequency::fromHz(123.0).hz(), 123.0);
        ASSERT_EQ(Frequency::fromKHz(2.0).hz(), 2'000.0);
        ASSERT_EQ(Frequency::fromMHz(3.0).hz(), 3'000'000.0);
        ASSERT_EQ(Frequency::fromGHz(4.0).hz(), 4'000'000'000.0);
    }

    void testUnitConversions()
    {
        constexpr Frequency frequency = Frequency::fromGHz(2.5);

        ASSERT_EQ(frequency.GHz(), 2.5);
        ASSERT_EQ(frequency.MHz(), 2'500.0);
        ASSERT_EQ(frequency.kHz(), 2'500'000.0);
        ASSERT_EQ(frequency.hz(), 2'500'000'000.0);
    }

    void testComparisonOperators()
    {
        constexpr Frequency first = Frequency::fromMHz(10);
        constexpr Frequency second = Frequency::fromMHz(20);

        ASSERT_TRUE(first < second);
        ASSERT_TRUE(second > first);
        ASSERT_TRUE(first != second);
        ASSERT_TRUE(first == first);
    }

    void testAddition()
    {
        constexpr Frequency first = Frequency::fromMHz(10);
        constexpr Frequency second = Frequency::fromMHz(20);
        constexpr Frequency result = first + second;

        ASSERT_EQ(result.MHz(), 30.0);
    }

    void testSubtraction()
    {
        constexpr Frequency first = Frequency::fromMHz(30);
        constexpr Frequency second = Frequency::fromMHz(10);
        constexpr Frequency result = first - second;

        ASSERT_EQ(result.MHz(), 20.0);
    }

    void testScalarMultiplication()
    {
        constexpr Frequency frequency = Frequency::fromMHz(10);
        ASSERT_EQ((frequency * 2.0).MHz(), 20.0);
        ASSERT_EQ((2.0 * frequency).MHz(), 20.0);
    }

    void testScalarDivision()
    {
        constexpr Frequency frequency = Frequency::fromMHz(20);

        ASSERT_EQ((frequency / 2.0).MHz(), 10.0);
    }

    void testRatio()
    {
        constexpr Frequency first = Frequency::fromMHz(20);
        constexpr Frequency second = Frequency::fromMHz(10);
        ASSERT_EQ(first / second, 2.0);
    }

    void testCompoundOperators()
    {
        auto frequency = Frequency::fromMHz(10);

        frequency += Frequency::fromMHz(5);
        ASSERT_EQ(frequency.MHz(), 15.0);

        frequency -= Frequency::fromMHz(10);
        ASSERT_EQ(frequency.MHz(), 5.0);

        frequency *= 4.0;
        ASSERT_EQ(frequency.MHz(), 20.0);

        frequency /= 2.0;
        ASSERT_EQ(frequency.MHz(), 10.0);
    }

    void testZero()
    {
        ASSERT_TRUE(Frequency().isZero());
        ASSERT_TRUE(Frequency::fromHz(0.0).isZero());
        ASSERT_FALSE(Frequency::fromHz(1.0).isZero());
    }

    void testPositive()
    {
        ASSERT_TRUE(Frequency::fromHz(1.0).isPositive());
        ASSERT_FALSE(Frequency::fromHz(-1.0).isPositive());
    }

    void testNegative()
    {
        ASSERT_TRUE(Frequency::fromHz(-1.0).isNegative());
        ASSERT_FALSE(Frequency::fromHz(1.0).isNegative());
    }

    void testUserDefinedLiterals()
    {
        ASSERT_EQ((10_Hz).hz(), 10.0);
        ASSERT_EQ((15_kHz).hz(), 15'000.0);
        ASSERT_EQ((12_MHz).hz(), 12'000'000.0);
        ASSERT_EQ((3_GHz).hz(), 3'000'000'000.0);
    }

    constexpr bool testConstexprImpl()
    {
        constexpr auto frequency = 100_MHz;
        return frequency.hz() == 100'000'000.0;
    }

    void testConstexpr()
    {
        static_assert(testConstexprImpl());
        ASSERT_TRUE(testConstexprImpl());
    }
}

void frequency_test()
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

    std::cout << "All Frequency tests passed.\n";
}