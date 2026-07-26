/**============================================================================
Name        : temperature.hpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : temperature.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_TEMPERATURE_HPP
#define CPPFIRMWAREEMBEDDED_TEMPERATURE_HPP

#include "detail/relative_quantity.hpp"

/**
 * ============================================================================
 * @file    temperature.hpp
 * @brief   Strongly typed representation of thermodynamic temperature and
 *          temperature difference.
 *
 * ----------------------------------------------------------------------------
 * Overview
 * ----------------------------------------------------------------------------
 *
 * This file defines two closely related types:
 *
 *      • Temperature
 *      • TemperatureDifference
 *
 * Temperature represents an absolute thermodynamic temperature, while
 * TemperatureDifference represents a relative temperature interval.
 *
 * The separation prevents accidental mixing of absolute temperatures and
 * temperature differences while allowing natural arithmetic between them.
 *
 * The design is inspired by std::chrono, where time_point and duration are
 * represented as different types with well-defined arithmetic.
 *
 * Example:
 *
 *      using namespace units::literals;
 *
 *      constexpr Temperature ambient = 25.0_degC;
 *      constexpr TemperatureDifference heating = 40.0_K;
 *
 *      constexpr Temperature cpu = ambient + heating;
 *      constexpr TemperatureDifference rise = cpu - ambient;
 *
 * ----------------------------------------------------------------------------
 * Internal representation
 * ----------------------------------------------------------------------------
 *
 * Both Temperature and TemperatureDifference are stored internally in Kelvin.
 *
 * Temperature stores the absolute thermodynamic temperature.
 *
 * TemperatureDifference stores only a temperature interval and therefore does
 * not have an absolute reference point.
 *
 *      25 °C
 *          ↓
 *      298.15 K
 *
 * ----------------------------------------------------------------------------
 * Supported units
 * ----------------------------------------------------------------------------
 *
 * Temperature
 *
 *      • K
 *      • °C
 *      • °F
 *
 * TemperatureDifference
 *
 *      • K
 *      • °C
 *      • °F
 *
 * ----------------------------------------------------------------------------
 * Domain model
 * ----------------------------------------------------------------------------
 *
 * Temperature and TemperatureDifference intentionally model different physical
 * concepts.
 *
 * Valid operations include:
 *
 *      Temperature + TemperatureDifference
 *      Temperature - TemperatureDifference
 *      Temperature - Temperature
 *      TemperatureDifference + TemperatureDifference
 *      TemperatureDifference - TemperatureDifference
 *
 * Invalid operations such as adding two absolute temperatures are intentionally
 * prohibited by the type system.
 *
 * ----------------------------------------------------------------------------
 * Design goals
 * ----------------------------------------------------------------------------
 *
 *  • Strong type safety
 *  • Zero runtime overhead
 *  • constexpr-friendly
 *  • Header-only implementation
 *  • Clear distinction between absolute and relative quantities
 *  • Suitable for firmware, embedded software, industrial control and
 *    scientific applications
 *
 * ----------------------------------------------------------------------------
 * References
 * ----------------------------------------------------------------------------
 *
 *  • ISO 80000-5 — Quantities and Units — Thermodynamics
 *  • The International System of Units (SI Brochure, 9th Edition)
 *  • std::chrono (ISO C++ Standard Library)
 *
 * ============================================================================
 */

namespace units
{
    class TemperatureDifference : public detail::RelativeQuantity<TemperatureDifference>
    {
    public:

        using ValueType = double;

        static constexpr ValueType CELSIUS_TO_KELVIN = 273.15;
        static constexpr ValueType FAHRENHEIT_TO_KELVIN_SCALE = 5.0 / 9.0;

        constexpr TemperatureDifference() = default;

    public:

        [[nodiscard]]
        static constexpr TemperatureDifference fromKelvin(const ValueType value) noexcept {
            return TemperatureDifference(value);
        }

        [[nodiscard]]
        static constexpr TemperatureDifference fromCelsius(const ValueType value) noexcept {
            return TemperatureDifference(value);
        }

        [[nodiscard]]
        static constexpr TemperatureDifference fromFahrenheit(const ValueType value) noexcept {
            return TemperatureDifference(value * FAHRENHEIT_TO_KELVIN_SCALE);
        }

    public:

        [[nodiscard]]
        constexpr ValueType kelvin() const noexcept {
            return nativeValue();
        }

        [[nodiscard]]
        constexpr ValueType celsius() const noexcept {
            return nativeValue();
        }

        [[nodiscard]]
        constexpr ValueType fahrenheit() const noexcept {
            return nativeValue() / FAHRENHEIT_TO_KELVIN_SCALE;
        }

    private:

        using Base = detail::RelativeQuantity<TemperatureDifference>;
        friend Base;

        constexpr explicit TemperatureDifference(const ValueType value) noexcept : Base(value)
        {
        }
    };

    class Temperature
    {
    public:

        using ValueType = double;
        static constexpr ValueType CELSIUS_OFFSET = 273.15;
        static constexpr ValueType FAHRENHEIT_OFFSET = 459.67;
        static constexpr ValueType FAHRENHEIT_SCALE = 5.0 / 9.0;

    public:

        constexpr Temperature() = default;

    public:

        [[nodiscard]]
        static constexpr Temperature fromKelvin(const ValueType value) noexcept {
            return Temperature(value);
        }

        [[nodiscard]]
        static constexpr Temperature fromCelsius(const ValueType value) noexcept {
            return Temperature(value + CELSIUS_OFFSET);
        }

        [[nodiscard]]
        static constexpr Temperature fromFahrenheit(const ValueType value) noexcept {
            return Temperature((value + FAHRENHEIT_OFFSET) * FAHRENHEIT_SCALE);
        }

        [[nodiscard]]
        constexpr ValueType kelvin() const noexcept {
            return value_;
        }

        [[nodiscard]]
        constexpr ValueType celsius() const noexcept {
            return value_ - CELSIUS_OFFSET;
        }

        [[nodiscard]]
        constexpr ValueType fahrenheit() const noexcept {
            return value_ / FAHRENHEIT_SCALE - FAHRENHEIT_OFFSET;
        }

        [[nodiscard]]
        constexpr bool isZero() const noexcept {
            return value_ == 0.0;
        }

        [[nodiscard]]
        constexpr bool isPositive() const noexcept {
            return value_ > 0.0;
        }

        [[nodiscard]]
        constexpr bool isNegative() const noexcept {
            return value_ < 0.0;
        }

        constexpr Temperature& operator+=(const TemperatureDifference difference) noexcept
        {
            value_ += difference.kelvin();
            return *this;
        }

        constexpr Temperature& operator-=(const TemperatureDifference difference) noexcept
        {
            value_ -= difference.kelvin();
            return *this;
        }

        [[nodiscard]]
        constexpr bool operator==(const Temperature& other) const noexcept {
            return value_ == other.value_;
        }

        [[nodiscard]]
        constexpr bool operator!=(const Temperature& other) const noexcept {
            return value_ != other.value_;
        }

        [[nodiscard]]
        constexpr bool operator<(const Temperature& other) const noexcept {
            return value_ < other.value_;
        }

        [[nodiscard]]
        constexpr bool operator<=(const Temperature& other) const noexcept {
            return value_ <= other.value_;
        }

        [[nodiscard]]
        constexpr bool operator>(const Temperature& other) const noexcept {
            return value_ > other.value_;
        }

        [[nodiscard]]
        constexpr bool operator>=(const Temperature& other) const noexcept {
            return value_ >= other.value_;
        }

    private:

        constexpr explicit Temperature(const ValueType value) noexcept
            : value_(value)
        {
        }

    private:

        ValueType value_ {};
    };

    [[nodiscard]]
    constexpr Temperature operator+(const Temperature temperature,
                                    const TemperatureDifference difference) noexcept
    {
        return Temperature::fromKelvin(temperature.kelvin() + difference.kelvin());
    }

    [[nodiscard]]
    constexpr Temperature operator+(const TemperatureDifference difference,
                                    const Temperature temperature) noexcept
    {
        return Temperature::fromKelvin(temperature.kelvin() + difference.kelvin());
    }

    [[nodiscard]]
    constexpr Temperature operator-(const Temperature temperature,
                                    const TemperatureDifference difference) noexcept
    {
        return Temperature::fromKelvin(temperature.kelvin() - difference.kelvin());
    }

    [[nodiscard]]
    constexpr TemperatureDifference operator-(const Temperature lhs,
                                              const Temperature rhs) noexcept
    {
        return TemperatureDifference::fromKelvin(lhs.kelvin() - rhs.kelvin());
    }
}

namespace units::literals
{
    [[nodiscard]]
    constexpr Temperature operator""_K(const long double value) noexcept
    {
        return Temperature::fromKelvin(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Temperature operator""_K(const unsigned long long value) noexcept
    {
        return Temperature::fromKelvin(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Temperature operator""_degC(const long double value) noexcept
    {
        return Temperature::fromCelsius(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Temperature operator""_degC(const unsigned long long value) noexcept
    {
        return Temperature::fromCelsius(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Temperature operator""_degF(const long double value) noexcept
    {
        return Temperature::fromFahrenheit(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Temperature operator""_degF(const unsigned long long value) noexcept
    {
        return Temperature::fromFahrenheit(static_cast<double>(value));
    }


    //=============================================================================
    // Temperature difference literals
    //=============================================================================

    [[nodiscard]]
    constexpr TemperatureDifference operator""_dK(const long double value) noexcept
    {
        return TemperatureDifference::fromKelvin(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr TemperatureDifference operator""_dK(const unsigned long long value) noexcept
    {
        return TemperatureDifference::fromKelvin(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr TemperatureDifference operator""_ddegC(const long double value) noexcept
    {
        return TemperatureDifference::fromCelsius(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr TemperatureDifference operator""_ddegC(const unsigned long long value) noexcept
    {
        return TemperatureDifference::fromCelsius(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr TemperatureDifference operator""_ddegF(const long double value) noexcept
    {
        return TemperatureDifference::fromFahrenheit(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr TemperatureDifference operator""_ddegF(const unsigned long long value) noexcept
    {
        return TemperatureDifference::fromFahrenheit(static_cast<double>(value));
    }
}

#endif //CPPFIRMWAREEMBEDDED_TEMPERATURE_HPP
