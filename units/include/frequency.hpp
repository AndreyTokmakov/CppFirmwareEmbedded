/**============================================================================
Name        : frequency.hpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : frequency.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_FREQUENCY_HPP
#define CPPFIRMWAREEMBEDDED_FREQUENCY_HPP

#include "detail/relative_quantity.hpp"

#pragma once

/**
 * ============================================================================
 * @file    frequency.hpp
 * @brief   Strongly typed representation of frequency.
 *
 * ----------------------------------------------------------------------------
 * Overview
 * ----------------------------------------------------------------------------
 *
 * This file defines the Frequency class used to represent frequency in a
 * type-safe manner.
 *
 * Frequency is one of the most frequently used physical quantities in firmware,
 * embedded software, digital signal processing (DSP), radio frequency (RF),
 * FPGA development and telecommunications.
 *
 * Instead of representing frequency as a raw floating-point value, this class
 * provides a dedicated type with explicit units, safe conversions and
 * domain-specific operations.
 *
 * Example:
 *
 *      using namespace units::literals;
 *
 *      constexpr Frequency sampleRate = 48.0_kHz;
 *      constexpr Frequency carrier = 915.0_MHz;
 *
 *      if (carrier > sampleRate)
 *      {
 *          ...
 *      }
 *
 * ----------------------------------------------------------------------------
 * Internal representation
 * ----------------------------------------------------------------------------
 *
 * Frequency is always stored internally in Hertz (Hz).
 *
 * All conversions are performed relative to the internal Hertz representation.
 *
 *      915 MHz
 *          ↓
 *      915000000 Hz
 *
 * ----------------------------------------------------------------------------
 * Supported units
 * ----------------------------------------------------------------------------
 *
 *      • Hz
 *      • kHz
 *      • MHz
 *      • GHz
 *
 * ----------------------------------------------------------------------------
 * Domain specific operations
 * ----------------------------------------------------------------------------
 *
 * Besides ordinary arithmetic inherited from RelativeQuantity, Frequency
 * provides several operations commonly required in firmware and RF software.
 *
 *      period()
 *          Calculates the period of the signal.
 *
 *      wavelength()
 *          Calculates the electromagnetic wavelength in vacuum.
 *
 * Additional operations can be added later without affecting the common
 * quantity interface.
 *
 * ----------------------------------------------------------------------------
 * Related quantities
 * ----------------------------------------------------------------------------
 *
 * Frequency is closely related to:
 *
 *      • Duration
 *      • Length
 *      • Bandwidth
 *      • SampleRate
 *      • AngularFrequency
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
 *  • Suitable for firmware, DSP, RF and embedded software
 *
 * ----------------------------------------------------------------------------
 * References
 * ----------------------------------------------------------------------------
 *
 *  • ISO 80000-3 — Quantities and units — Space and time
 *  • IEC 60050 — International Electrotechnical Vocabulary
 *  • IEEE Std 145-2013 — Standard for Definitions of Terms for Antennas
 *
 * ============================================================================
 */

namespace units
{
    class Frequency : public detail::RelativeQuantity<Frequency>
    {
    public:

        using ValueType = double;

    private:

        using Base = detail::RelativeQuantity<Frequency>;

    public:

        static constexpr ValueType HZ_PER_KHZ = 1'000.0;
        static constexpr ValueType HZ_PER_MHZ = 1'000'000.0;
        static constexpr ValueType HZ_PER_GHZ = 1'000'000'000.0;
        static constexpr ValueType SPEED_OF_LIGHT = 299'792'458.0;

        constexpr Frequency() = default;


        [[nodiscard]]
        static constexpr Frequency fromHz(const ValueType value) noexcept {
            return Frequency(value);
        }

        [[nodiscard]]
        static constexpr Frequency fromKHz(const ValueType value) noexcept {
            return Frequency(value * HZ_PER_KHZ);
        }

        [[nodiscard]]
        static constexpr Frequency fromMHz(const ValueType value) noexcept {
            return Frequency(value * HZ_PER_MHZ);
        }

        [[nodiscard]]
        static constexpr Frequency fromGHz(const ValueType value) noexcept {
            return Frequency(value * HZ_PER_GHZ);
        }

    public:

        [[nodiscard]]
        constexpr ValueType hz() const noexcept {
            return nativeValue();
        }

        [[nodiscard]]
        constexpr ValueType kHz() const noexcept {
            return nativeValue() / HZ_PER_KHZ;
        }

        [[nodiscard]]
        constexpr ValueType MHz() const noexcept {
            return nativeValue() / HZ_PER_MHZ;
        }

        [[nodiscard]]
        constexpr ValueType GHz() const noexcept {
            return nativeValue() / HZ_PER_GHZ;
        }
#if 0
    public:

        [[nodiscard]]
        Duration period() const noexcept;

        [[nodiscard]]
        Length wavelength() const noexcept;
#endif

    private:

        friend Base;

        constexpr explicit Frequency(const ValueType value) noexcept : Base(value)
        {
        }
    };
}

namespace units::literals
{
    [[nodiscard]]
    constexpr Frequency operator""_Hz(const long double value) noexcept {
        return Frequency::fromHz(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Frequency operator""_Hz(const unsigned long long value) noexcept {
        return Frequency::fromHz(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Frequency operator""_kHz(const long double value) noexcept{
        return Frequency::fromKHz(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Frequency operator""_kHz(const unsigned long long value) noexcept {
        return Frequency::fromKHz(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Frequency operator""_MHz(const long double value) noexcept {
        return Frequency::fromMHz(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Frequency operator""_MHz(const unsigned long long value) noexcept {
        return Frequency::fromMHz(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Frequency operator""_GHz(const long double value) noexcept {
        return Frequency::fromGHz(static_cast<double>(value));
    }

    [[nodiscard]]
    constexpr Frequency operator""_GHz(const unsigned long long value) noexcept{
        return Frequency::fromGHz(static_cast<double>(value));
    }
}

#endif //CPPFIRMWAREEMBEDDED_FREQUENCY_HPP
