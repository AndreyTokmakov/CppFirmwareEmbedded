/**============================================================================
Name        : relative_quantity.hpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : frequency.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_RELATIVEQUANTITY_HPP
#define CPPFIRMWAREEMBEDDED_RELATIVEQUANTITY_HPP

#include <compare>

/**
 * ============================================================================
 * @file    relative_quantity.hpp
 * @brief   Common CRTP base class for relative physical quantities.
 *
 * ----------------------------------------------------------------------------
 * Overview
 * ----------------------------------------------------------------------------
 *
 * This file defines RelativeQuantity, a CRTP base class that implements common
 * arithmetic and comparison operations shared by most physical quantities.
 *
 * Relative quantities represent values that can be freely added, subtracted,
 * multiplied or divided by scalar values.
 *
 * Typical examples include:
 *
 *      • Frequency
 *      • Voltage
 *      • Current
 *      • Resistance
 *      • Capacitance
 *      • Inductance
 *      • Power
 *      • Energy
 *      • Charge
 *      • Duration
 *      • Bandwidth
 *      • SampleRate
 *
 * Unlike absolute quantities (such as Temperature), relative quantities have
 * no origin or offset and therefore support ordinary arithmetic.
 *
 * ----------------------------------------------------------------------------
 * Why CRTP?
 * ----------------------------------------------------------------------------
 *
 * Almost every relative quantity provides exactly the same arithmetic
 * operators.
 *
 * Without a common base class each quantity would need to implement:
 *
 *      • comparison operators
 *      • addition
 *      • subtraction
 *      • scalar multiplication
 *      • scalar division
 *      • ratio calculation
 *      • zero checks
 *      • sign checks
 *
 * This class eliminates code duplication while preserving strong typing.
 *
 * ----------------------------------------------------------------------------
 * Design goals
 * ----------------------------------------------------------------------------
 *
 *  • Zero runtime overhead
 *  • constexpr-friendly
 *  • Header-only implementation
 *  • Strong type safety
 *  • No virtual functions
 *  • Trivially copyable
 *  • Suitable for firmware and embedded software
 *
 * ----------------------------------------------------------------------------
 * Usage
 * ----------------------------------------------------------------------------
 *
 * Example:
 *
 *      class Frequency
 *          : public units::detail::RelativeQuantity<Frequency>
 *      {
 *          ...
 *      };
 *
 * Derived classes are expected to provide:
 *
 *      • factory methods
 *      • unit conversions
 *      • domain specific operations
 *
 * RelativeQuantity only implements common arithmetic.
 *
 * ============================================================================
 */

namespace units::detail
{

    template<typename Derived, typename ValueType = double>
    struct  RelativeQuantity
    {
    protected:

        constexpr RelativeQuantity() = default;

        constexpr explicit RelativeQuantity(const ValueType value) noexcept: value(value) {
        }

    public:

        [[nodiscard]]
        constexpr bool isZero() const noexcept {
            return value == ValueType {};
        }

        [[nodiscard]]
        constexpr bool isPositive() const noexcept {
            return value > ValueType {};
        }

        [[nodiscard]]
        constexpr bool isNegative() const noexcept {
            return value < ValueType {};
        }

        constexpr auto operator<=>(const RelativeQuantity&) const = default;

        [[nodiscard]]
        constexpr Derived operator+(const Derived rhs) const noexcept {
            return Derived(value + rhs.getValue());
        }

        [[nodiscard]]
        constexpr Derived operator-(const Derived rhs) const noexcept {
            return Derived(value - rhs.getValue());
        }

        constexpr Derived& operator+=(const Derived rhs) noexcept
        {
            value += rhs.getValue();
            return self();
        }

        constexpr Derived& operator-=(const Derived rhs) noexcept
        {
            value -= rhs.getValue();
            return self();
        }

        [[nodiscard]]
        constexpr Derived operator*(const ValueType val) const noexcept {
            return Derived(value * val);
        }

        [[nodiscard]]
        constexpr Derived operator/(const ValueType val) const noexcept {
            return Derived(value / val);
        }

        constexpr Derived& operator*=(const ValueType val) noexcept
        {
            value *= val;
            return self();
        }

        constexpr Derived& operator/=(const ValueType val) noexcept
        {
            value /= val;
            return self();
        }

        [[nodiscard]]
        constexpr ValueType operator/(const Derived rhs) const noexcept {
            return value / rhs.getValue();
        }

    protected:

        [[nodiscard]]
        constexpr ValueType getValue() const noexcept {
            return value;
        }

        [[nodiscard]]
        constexpr Derived& self() noexcept {
            return static_cast<Derived&>(*this);
        }

        [[nodiscard]]
        constexpr const Derived& self() const noexcept {
            return static_cast<const Derived&>(*this);
        }

    private:

        ValueType value {};
    };

    template<typename Derived, typename ValueType>
    [[nodiscard]]
    constexpr Derived operator*(const ValueType lhs,
                                const RelativeQuantity<Derived, ValueType>& rhs) noexcept
    {
        return static_cast<const Derived&>(rhs) * lhs;
    }

} // namespace units::detail

#endif //CPPFIRMWAREEMBEDDED_RELATIVEQUANTITY_HPP
