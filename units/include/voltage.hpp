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
 * Voltage is one of the fundamental electrical quantities used throughout
 * firmware, embedded software, electronics, power systems, FPGA development
 * and hardware control applications.
 *
 * Instead of representing voltage as a raw floating-point value, this class
 * provides explicit units, safe conversions and compile-time type safety.
 *
 * Voltage is a relative quantity and therefore supports ordinary arithmetic
 * through the RelativeQuantity base class.
 *
 * Example:
 *
 *      using namespace units::literals;
 *
 *      constexpr Voltage coreVoltage = 1.2_V;
 *      constexpr Voltage battery = 12.0_V;
 *      constexpr Voltage reference = 3300_mV;
 *
 *      if (battery > coreVoltage)
 *      {
 *          ...
 *      }
 *
 * ----------------------------------------------------------------------------
 * Internal representation
 * ----------------------------------------------------------------------------
 *
 * Voltage is always stored internally in Volts (V).
 *
 * All conversions are performed relative to the internal Volt representation.
 *
 *      3300 mV
 *          ↓
 *      3.3 V
 *
 * ----------------------------------------------------------------------------
 * Supported units
 * ----------------------------------------------------------------------------
 *
 *      • nV
 *      • uV
 *      • mV
 *      • V
 *      • kV
 *
 * ----------------------------------------------------------------------------
 * Domain specific operations
 * ----------------------------------------------------------------------------
 *
 * Voltage intentionally contains only functionality directly related to voltage
 * itself.
 *
 * Relationships between voltage and other physical quantities such as current,
 * resistance and power are implemented as free functions in dedicated relation
 * headers. This keeps the class independent from other unit types and avoids
 * cyclic header dependencies.
 *
 * ----------------------------------------------------------------------------
 * Design goals
 * ----------------------------------------------------------------------------
 *
 *  • Strong type safety
 *  • Zero runtime overhead
 *  • constexpr-friendly
 *  • Header-only implementation
 *  • Trivially copyable
 *  • Common arithmetic via RelativeQuantity
 *  • Suitable for firmware, embedded software and electronics
 *
 * ----------------------------------------------------------------------------
 * References
 * ----------------------------------------------------------------------------
 *
 *  • ISO 80000-6 — Quantities and Units — Electromagnetism
 *  • IEC 60050 — International Electrotechnical Vocabulary
 *  • The International System of Units (SI Brochure, 9th Edition)
 *
 * ============================================================================
 */

#include "detail/relative_quantity.hpp"

namespace units
{
    class Voltage : public detail::RelativeQuantity<Voltage>
    {
    public:

        using ValueType = double;

    public:

        static constexpr ValueType VOLTS_PER_NANOVOLT = 1e-9;
        static constexpr ValueType VOLTS_PER_MICROVOLT = 1e-6;
        static constexpr ValueType VOLTS_PER_MILLIVOLT = 1e-3;
        static constexpr ValueType VOLTS_PER_KILOVOLT = 1e3;

        constexpr Voltage() = default;

        [[nodiscard]]
        static constexpr Voltage fromVolts(const ValueType value) noexcept {
            return Voltage(value);
        }

        [[nodiscard]]
        static constexpr Voltage fromMilliVolts(const ValueType value) noexcept {
            return Voltage(value * VOLTS_PER_MILLIVOLT);
        }

        [[nodiscard]]
        static constexpr Voltage fromMicroVolts(const ValueType value) noexcept {
            return Voltage(value * VOLTS_PER_MICROVOLT);
        }

        [[nodiscard]]
        static constexpr Voltage fromNanoVolts(const ValueType value) noexcept {
            return Voltage(value * VOLTS_PER_NANOVOLT);
        }

        [[nodiscard]]
        static constexpr Voltage fromKiloVolts(const ValueType value) noexcept {
            return Voltage(value * VOLTS_PER_KILOVOLT);
        }

        [[nodiscard]]
        constexpr ValueType volts() const noexcept {
            return nativeValue();
        }

        [[nodiscard]]
        constexpr ValueType milliVolts() const noexcept {
            return nativeValue() / VOLTS_PER_MILLIVOLT;
        }

        [[nodiscard]]
        constexpr ValueType microVolts() const noexcept {
            return nativeValue() / VOLTS_PER_MICROVOLT;
        }

        [[nodiscard]]
        constexpr ValueType nanoVolts() const noexcept {
            return nativeValue() / VOLTS_PER_NANOVOLT;
        }

        [[nodiscard]]
        constexpr ValueType kiloVolts() const noexcept {
            return nativeValue() / VOLTS_PER_KILOVOLT;
        }

    private:

        using Base = detail::RelativeQuantity<Voltage>;
        friend Base;

        constexpr explicit Voltage(const ValueType value) noexcept : Base(value)
        {
        }
    };

}

namespace units::literals
{
    [[nodiscard]]
    constexpr Voltage operator""_V(const long double value) noexcept
    {
        return Voltage::fromVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_V(const unsigned long long value) noexcept
    {
        return Voltage::fromVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_mV(const long double value) noexcept
    {
        return Voltage::fromMilliVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_mV(const unsigned long long value) noexcept
    {
        return Voltage::fromMilliVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_uV(const long double value) noexcept
    {
        return Voltage::fromMicroVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_uV(const unsigned long long value) noexcept
    {
        return Voltage::fromMicroVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_nV(const long double value) noexcept
    {
        return Voltage::fromNanoVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_nV(const unsigned long long value) noexcept
    {
        return Voltage::fromNanoVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_kV(const long double value) noexcept
    {
        return Voltage::fromKiloVolts(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Voltage operator""_kV(const unsigned long long value) noexcept
    {
        return Voltage::fromKiloVolts(static_cast<double>(value));
    }
}

#endif //CPPFIRMWAREEMBEDDED_VOLTAGE_HPP
