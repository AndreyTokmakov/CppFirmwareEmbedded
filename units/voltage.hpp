/**============================================================================
Name        : voltage.hpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : voltage.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_VOLTAGE_HPP
#define CPPFIRMWAREEMBEDDED_VOLTAGE_HPP

#pragma once

/**
 * ============================================================================
 * @file    voltage.hpp
 * @brief   Strongly typed representation of electrical voltage.
 *
 * ----------------------------------------------------------------------------
 * Overview
 * ----------------------------------------------------------------------------
 *
 * This file defines the Voltage class used to represent electrical potential
 * difference in a type-safe manner.
 *
 * Voltage values are stored internally in Volts (V) while providing convenient
 * conversions between commonly used units.
 *
 * Using a dedicated type instead of raw numeric values makes firmware APIs
 * significantly safer and easier to understand.
 *
 * Example:
 *
 *      void setReferenceVoltage(Voltage voltage);
 *
 * instead of
 *
 *      void setReferenceVoltage(double voltage);
 *
 * ----------------------------------------------------------------------------
 * Internal representation
 * ----------------------------------------------------------------------------
 *
 * Voltage is always stored internally in Volts.
 *
 *      3300 mV
 *          ↓
 *      3.3 V
 *
 * ----------------------------------------------------------------------------
 * Supported units
 * ----------------------------------------------------------------------------
 *
 *  nV
 *  uV
 *  mV
 *  V
 *  kV
 *
 * ----------------------------------------------------------------------------
 * Example
 * ----------------------------------------------------------------------------
 *
 *      using namespace units::literals;
 *
 *      Voltage reference = 3.3_V;
 *      Voltage threshold = 250_mV;
 *
 *      if (reference > threshold)
 *      {
 *          ...
 *      }
 *
 * ----------------------------------------------------------------------------
 * Design goals
 * ----------------------------------------------------------------------------
 *
 *  • constexpr-friendly
 *  • zero runtime overhead
 *  • header-only
 *  • trivially copyable
 *  • suitable for firmware and embedded applications
 *
 * ============================================================================
 */

#include <compare>

namespace units
{

    class Current;
    class Resistance;
    class Power;

    class Voltage
    {
    public:

        using ValueType = double;

        static constexpr ValueType VoltsPerMilliVolt = 1e-3;
        static constexpr ValueType VoltsPerMicroVolt = 1e-6;
        static constexpr ValueType VoltsPerNanoVolt  = 1e-9;
        static constexpr ValueType VoltsPerKiloVolt  = 1e3;

    public:

        constexpr Voltage() = default;

        [[nodiscard]]
        static constexpr Voltage fromVolts(const ValueType value) noexcept {
            return Voltage(value);
        }

        [[nodiscard]]
        static constexpr Voltage fromMilliVolts(const ValueType value) noexcept {
            return Voltage(value * VoltsPerMilliVolt);
        }

        [[nodiscard]]
        static constexpr Voltage fromMicroVolts(const ValueType value) noexcept{
            return Voltage(value * VoltsPerMicroVolt);
        }

        [[nodiscard]]
        static constexpr Voltage fromNanoVolts(const ValueType value) noexcept {
            return Voltage(value * VoltsPerNanoVolt);
        }

        [[nodiscard]]
        static constexpr Voltage fromKiloVolts(const ValueType value) noexcept {
            return Voltage(value * VoltsPerKiloVolt);
        }

    public:

        [[nodiscard]]
        constexpr ValueType toVolts() const noexcept {
            return volts_;
        }

        [[nodiscard]]
        constexpr ValueType toMilliVolts() const noexcept {
            return volts_ / VoltsPerMilliVolt;
        }

        [[nodiscard]]
        constexpr ValueType toMicroVolts() const noexcept {
            return volts_ / VoltsPerMicroVolt;
        }

        [[nodiscard]]
        constexpr ValueType toNanoVolts() const noexcept {
            return volts_ / VoltsPerNanoVolt;
        }

        [[nodiscard]]
        constexpr ValueType toKiloVolts() const noexcept {
            return volts_ / VoltsPerKiloVolt;
        }

    public:

        [[nodiscard]]
        constexpr bool isZero() const noexcept {
            return volts_ == 0.0;
        }

        [[nodiscard]]
        constexpr bool isPositive() const noexcept {
            return volts_ > 0.0;
        }

    public:

        constexpr auto operator<=>(const Voltage&) const = default;

    public:

        [[nodiscard]]
        constexpr Voltage operator+(const Voltage rhs) const noexcept {
            return Voltage(volts_ + rhs.volts_);
        }

        [[nodiscard]]
        constexpr Voltage operator-(const Voltage rhs) const noexcept {
            return Voltage(volts_ - rhs.volts_);
        }

        constexpr Voltage& operator+=(const Voltage rhs) noexcept
        {
            volts_ += rhs.volts_;
            return *this;
        }

        constexpr Voltage& operator-=(const Voltage rhs) noexcept
        {
            volts_ -= rhs.volts_;
            return *this;
        }

    public:

        [[nodiscard]]
        constexpr Voltage operator*(const ValueType value) const noexcept {
            return Voltage(volts_ * value);
        }

        [[nodiscard]]
        constexpr Voltage operator/(const ValueType value) const noexcept {
            return Voltage(volts_ / value);
        }

        constexpr Voltage& operator*=(const ValueType value) noexcept
        {
            volts_ *= value;
            return *this;
        }

        constexpr Voltage& operator/=(const ValueType value) noexcept
        {
            volts_ /= value;
            return *this;
        }

    public:

        /**
         * Returns a dimensionless ratio.
         */
        [[nodiscard]]
        constexpr ValueType operator/(const Voltage rhs) const noexcept
        {
            return volts_ / rhs.volts_;
        }

    public:

        /**
         * Calculates electrical power for a given resistance.
         *      P = V² / R
         */
        [[nodiscard]]
        Power power(Resistance resistance) const noexcept;

        /**
         * Calculates electrical current for a given resistance.
         *      I = V / R
         */
        [[nodiscard]]
        Current current(Resistance resistance) const noexcept;

    private:

        constexpr explicit Voltage(const ValueType volts) noexcept: volts_(volts)
        {
        }

    private:

        ValueType volts_ {};
    };

    constexpr Voltage operator*(const Voltage::ValueType lhs, const Voltage rhs) noexcept{
        return rhs * lhs;
    }

} // namespace units

namespace units::literals
{
    constexpr Voltage operator""_V(const long double value)
    {
        return Voltage::fromVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_V(const unsigned long long value)
    {
        return Voltage::fromVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_mV(const long double value)
    {
        return Voltage::fromMilliVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_mV(const unsigned long long value)
    {
        return Voltage::fromMilliVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_uV(const long double value)
    {
        return Voltage::fromMicroVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_uV(const unsigned long long value)
    {
        return Voltage::fromMicroVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_nV(const long double value)
    {
        return Voltage::fromNanoVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_nV(const unsigned long long value)
    {
        return Voltage::fromNanoVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_kV(const long double value)
    {
        return Voltage::fromKiloVolts(static_cast<double>(value));
    }

    constexpr Voltage operator""_kV(const unsigned long long value)
    {
        return Voltage::fromKiloVolts(static_cast<double>(value));
    }
}

#endif //CPPFIRMWAREEMBEDDED_VOLTAGE_HPP
