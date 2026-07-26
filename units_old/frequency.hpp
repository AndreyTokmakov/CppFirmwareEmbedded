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

#include <compare>

/**
 * ============================================================================
 * @file    frequency.hpp
 * @brief   Strongly typed representation of frequency.
 *
 * ----------------------------------------------------------------------------
 * Overview
 * ----------------------------------------------------------------------------
 *
 * This file defines the Frequency class used to represent physical frequency
 * values in a type-safe manner.
 *
 * The class stores the value internally in Hertz (Hz) while providing
 * convenient factory methods, conversions and arithmetic operations.
 *
 * Unlike using raw double or float values throughout the firmware,
 * Frequency provides:
 *
 *  - compile-time type safety
 *  - self-documenting interfaces
 *  - readable code
 *  - automatic unit conversion
 *  - prevention of mixing incompatible physical quantities
 *
 * Example:
 *
 *      void configurePll(Frequency frequency);
 *
 * instead of
 *
 *      void configurePll(double value);
 *
 * which gives no indication about the expected unit.
 *
 * ----------------------------------------------------------------------------
 * Internal representation
 * ----------------------------------------------------------------------------
 *
 * Frequency is always stored internally in Hertz.
 *
 *      2.45 GHz
 *          ↓
 *      2'450'000'000 Hz
 *
 * All conversions are performed transparently.
 *
 * ----------------------------------------------------------------------------
 * Supported units
 * ----------------------------------------------------------------------------
 *
 *  Hz
 *  kHz
 *  MHz
 *  GHz
 *
 * Additional units may be added later without affecting existing code.
 *
 * ----------------------------------------------------------------------------
 * Example
 * ----------------------------------------------------------------------------
 *
 *      using namespace units::literals;
 *      Frequency cpuClock = 168_MHz;
 *      Frequency offset = 25_kHz;
 *      Frequency result = cpuClock + offset;
 *      if (result > 100_MHz)
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
 *  • trivially copyable
 *  • header-only
 *  • suitable for firmware
 *  • suitable for DSP
 *  • suitable for SDR
 *  • suitable for FPGA host software
 *
 * ----------------------------------------------------------------------------
 * Related classes
 * ----------------------------------------------------------------------------
 *
 *  Period
 *  Duration
 *  SampleRate
 *  Bandwidth
 *  Wavelength
 *  Power
 *  Gain
 *  Impedance
 *
 * ============================================================================
 */

namespace units
{

    class Period;
    class Wavelength;

    class Frequency
    {
    public:

        using ValueType = double;

        static constexpr ValueType HzPerKHz = 1'000.0;
        static constexpr ValueType HzPerMHz = 1'000'000.0;
        static constexpr ValueType HzPerGHz = 1'000'000'000.0;

    public:

        constexpr Frequency() = default;

        [[nodiscard]]
        static constexpr Frequency fromHz(const ValueType value) noexcept {
            return Frequency(value);
        }

        [[nodiscard]]
        static constexpr Frequency fromKHz(const ValueType value) noexcept {
            return Frequency(value * HzPerKHz);
        }

        [[nodiscard]]
        static constexpr Frequency fromMHz(const ValueType value) noexcept {
            return Frequency(value * HzPerMHz);
        }

        [[nodiscard]]
        static constexpr Frequency fromGHz(const ValueType value) noexcept {
            return Frequency(value * HzPerGHz);
        }

    public:

        [[nodiscard]]
        constexpr ValueType hz() const noexcept {
            return hz_;
        }

        [[nodiscard]]
        constexpr ValueType kHz() const noexcept {
            return hz_ / HzPerKHz;
        }

        [[nodiscard]]
        constexpr ValueType MHz() const noexcept {
            return hz_ / HzPerMHz;
        }

        [[nodiscard]]
        constexpr ValueType GHz() const noexcept {
            return hz_ / HzPerGHz;
        }

        [[nodiscard]]
        constexpr bool isZero() const noexcept {
            return hz_ == 0.0;
        }

        [[nodiscard]]
        constexpr bool isPositive() const noexcept {
            return hz_ > 0.0;
        }

    public:

        constexpr auto operator<=>(const Frequency&) const = default;

    public:

        [[nodiscard]]
        constexpr Frequency operator+(const Frequency rhs) const noexcept {
            return Frequency(hz_ + rhs.hz_);
        }

        [[nodiscard]]
        constexpr Frequency operator-(const Frequency rhs) const noexcept {
            return Frequency(hz_ - rhs.hz_);
        }

        constexpr Frequency& operator+=(const Frequency rhs) noexcept
        {
            hz_ += rhs.hz_;
            return *this;
        }

        constexpr Frequency& operator-=(const Frequency rhs) noexcept
        {
            hz_ -= rhs.hz_;
            return *this;
        }

        [[nodiscard]]
        constexpr Frequency operator*(const ValueType value) const noexcept {
            return Frequency(hz_ * value);
        }

        [[nodiscard]]
        constexpr Frequency operator/(const ValueType value) const noexcept {
            return Frequency(hz_ / value);
        }

        constexpr Frequency& operator*=(const ValueType value) noexcept
        {
            hz_ *= value;
            return *this;
        }

        constexpr Frequency& operator/=(const ValueType value) noexcept
        {
            hz_ /= value;
            return *this;
        }

    public:

        /**
         * Returns a dimensionless ratio.
         *
         * Example:
         *
         *      auto ratio = 200_MHz / 100_MHz;
         *      // ratio == 2.0
         */
        [[nodiscard]]
        constexpr ValueType operator/(const Frequency rhs) const noexcept {
            return hz_ / rhs.hz_;
        }

    public:

        /**
         * Returns the period corresponding to the frequency.
         *
         * Example:
         *
         *      1 MHz -> 1 us
         */
        [[nodiscard]]
        Period period() const noexcept;

        /**
         * Calculates wavelength assuming propagation in vacuum.
         *
         * λ = c / f
         */
        [[nodiscard]]
        Wavelength wavelength() const noexcept;

    private:

        constexpr explicit Frequency(const ValueType hz) noexcept : hz_(hz)
        {
        }

    private:

        ValueType hz_ {};
    };

    constexpr Frequency operator*(const Frequency::ValueType lhs,
                                  const Frequency rhs) noexcept
    {
        return rhs * lhs;
    }

    } // namespace units

    namespace units::literals
    {

    constexpr Frequency operator""_Hz(const long double value)
    {
        return Frequency::fromHz(static_cast<double>(value));
    }

    constexpr Frequency operator""_Hz(const unsigned long long value)
    {
        return Frequency::fromHz(static_cast<double>(value));
    }

    constexpr Frequency operator""_kHz(const long double value)
    {
        return Frequency::fromKHz(static_cast<double>(value));
    }

    constexpr Frequency operator""_kHz(const unsigned long long value)
    {
        return Frequency::fromKHz(static_cast<double>(value));
    }

    constexpr Frequency operator""_MHz(const long double value)
    {
        return Frequency::fromMHz(static_cast<double>(value));
    }

    constexpr Frequency operator""_MHz(const unsigned long long value)
    {
        return Frequency::fromMHz(static_cast<double>(value));
    }

    constexpr Frequency operator""_GHz(const long double value)
    {
        return Frequency::fromGHz(static_cast<double>(value));
    }

    constexpr Frequency operator""_GHz(const unsigned long long value)
    {
        return Frequency::fromGHz(static_cast<double>(value));
    }
} // namespace units::literals

#endif //CPPFIRMWAREEMBEDDED_FREQUENCY_HPP
