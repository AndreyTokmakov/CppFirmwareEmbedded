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

/**
 * ============================================================================
 * @file    temperature.hpp
 * @brief   Strongly typed representation of absolute temperature and
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
 * Temperature represents an absolute thermodynamic temperature.
 *
 * TemperatureDifference represents a temperature interval (ΔT) between two
 * temperature values.
 *
 * Unlike most physical quantities, absolute temperatures cannot be added
 * together because such an operation has no physical meaning.
 *
 * Instead, the library follows a model similar to std::chrono where absolute
 * values and relative values are represented by different types.
 *
 * ----------------------------------------------------------------------------
 * Why two types?
 * ----------------------------------------------------------------------------
 *
 * Consider the following expression:
 *
 *      20 °C + 30 °C
 *
 * What should the result be?
 *
 * There is no correct physical interpretation.
 *
 * However, the following operations are well defined:
 *
 *      Temperature + TemperatureDifference
 *      Temperature - TemperatureDifference
 *      Temperature - Temperature
 *
 * The last operation produces a TemperatureDifference.
 *
 * This design prevents many classes of programming errors at compile time while
 * keeping the API intuitive.
 *
 * ----------------------------------------------------------------------------
 * Internal representation
 * ----------------------------------------------------------------------------
 *
 * Temperature is always stored internally in Kelvin.
 *
 * Kelvin is the SI base unit of thermodynamic temperature and allows all
 * conversions to be implemented consistently.
 *
 *      0 °C     = 273.15 K
 *      100 °C   = 373.15 K
 *
 * TemperatureDifference is also stored in Kelvin.
 *
 * Unlike absolute temperatures, temperature differences have no offset.
 *
 * Therefore:
 *
 *      1 K  == 1 °C (difference)
 *
 * but
 *
 *      1 K  != 1 °C (absolute value)
 *
 * ----------------------------------------------------------------------------
 * Supported units
 * ----------------------------------------------------------------------------
 *
 * Absolute temperatures:
 *
 *      • Kelvin
 *      • Celsius
 *      • Fahrenheit
 *
 * Temperature differences:
 *
 *      • Kelvin
 *      • Celsius
 *      • Fahrenheit
 *
 * ----------------------------------------------------------------------------
 * Example
 * ----------------------------------------------------------------------------
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
 * Design goals
 * ----------------------------------------------------------------------------
 *
 *  • Strong type safety
 *  • constexpr-friendly
 *  • Zero runtime overhead
 *  • Trivially copyable
 *  • Header-only implementation
 *  • Suitable for firmware, DSP and embedded software
 *  • Similar design philosophy to std::chrono
 *
 * ----------------------------------------------------------------------------
 * Invalid operations
 * ----------------------------------------------------------------------------
 *
 * The following operations are intentionally not supported:
 *
 *      Temperature + Temperature
 *      Temperature * scalar
 *      Temperature / scalar
 *
 * because they do not represent meaningful physical operations.
 *
 * ============================================================================
 */

namespace units
{
	class TemperatureDifference
	{
	public:

	    using ValueType = double;
	    static constexpr ValueType FahrenheitScale = 9.0 / 5.0;
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
	        return TemperatureDifference(value / FahrenheitScale);
	    }

	public:

	    [[nodiscard]]
	    constexpr ValueType kelvin() const noexcept {
	        return kelvin_;
	    }

	    [[nodiscard]]
	    constexpr ValueType celsius() const noexcept {
	        return kelvin_;
	    }

	    [[nodiscard]]
	    constexpr ValueType fahrenheit() const noexcept {
	        return kelvin_ * FahrenheitScale;
	    }

	public:

	    [[nodiscard]]
	    constexpr bool isZero() const noexcept {
	        return kelvin_ == 0.0;
	    }

	    [[nodiscard]]
	    constexpr TemperatureDifference abs() const noexcept {
	        return TemperatureDifference(kelvin_ < 0.0 ? -kelvin_ : kelvin_);
	    }

	public:

	    constexpr auto operator<=>(const TemperatureDifference&) const = default;

	public:

	    [[nodiscard]]
	    constexpr TemperatureDifference operator+(const TemperatureDifference rhs) const noexcept
	    {
	        return TemperatureDifference(kelvin_ + rhs.kelvin_);
	    }

	    [[nodiscard]]
	    constexpr TemperatureDifference operator-(const TemperatureDifference rhs) const noexcept{
	        return TemperatureDifference(kelvin_ - rhs.kelvin_);
	    }

	    constexpr TemperatureDifference& operator+=(const TemperatureDifference rhs) noexcept
	    {
	        kelvin_ += rhs.kelvin_;
	        return *this;
	    }

	    constexpr TemperatureDifference& operator-=(const TemperatureDifference rhs) noexcept
	    {
	        kelvin_ -= rhs.kelvin_;
	        return *this;
	    }

	public:

	    [[nodiscard]]
	    constexpr TemperatureDifference operator*(const ValueType value) const noexcept {
	        return TemperatureDifference(kelvin_ * value);
	    }

	    [[nodiscard]]
	    constexpr TemperatureDifference operator/(const ValueType value) const noexcept {
	        return TemperatureDifference(kelvin_ / value);
	    }

	    constexpr TemperatureDifference& operator*=(const ValueType value) noexcept
	    {
	        kelvin_ *= value;
	        return *this;
	    }

	    constexpr TemperatureDifference& operator/=(const ValueType value) noexcept
	    {
	        kelvin_ /= value;
	        return *this;
	    }

	    [[nodiscard]]
	    constexpr ValueType operator/(
	        const TemperatureDifference rhs) const noexcept
	    {
	        return kelvin_ / rhs.kelvin_;
	    }

	private:

	    constexpr explicit TemperatureDifference(const ValueType kelvin) noexcept: kelvin_(kelvin){
	    }

	private:

	    ValueType kelvin_ {};
	};

	inline constexpr TemperatureDifference operator*(
	    const TemperatureDifference::ValueType lhs,
	    const TemperatureDifference rhs) noexcept
	{
	    return rhs * lhs;
	}

	class Temperature
	{
	public:

	    using ValueType = double;

	public:

	    static constexpr ValueType KelvinOffset = 273.15;
	    static constexpr ValueType FahrenheitScale = 9.0 / 5.0;
	    static constexpr ValueType FahrenheitOffset = 32.0;

	public:

	    constexpr Temperature() = default;

	public:

	    [[nodiscard]]
	    static constexpr Temperature fromKelvin(const ValueType value) noexcept {
	        return Temperature(value);
	    }

	    [[nodiscard]]
	    static constexpr Temperature fromCelsius(const ValueType value) noexcept {
	        return Temperature(value + KelvinOffset);
	    }

	    [[nodiscard]]
	    static constexpr Temperature fromFahrenheit(const ValueType value) noexcept {
	        return Temperature((value - FahrenheitOffset) / FahrenheitScale + KelvinOffset);
	    }

	public:

	    [[nodiscard]]
	    constexpr ValueType kelvin() const noexcept {
	        return kelvin_;
	    }

	    [[nodiscard]]
	    constexpr ValueType celsius() const noexcept {
	        return kelvin_ - KelvinOffset;
	    }

	    [[nodiscard]]
	    constexpr ValueType fahrenheit() const noexcept {
	        return (kelvin_ - KelvinOffset) * FahrenheitScale + FahrenheitOffset;
	    }

	public:

	    [[nodiscard]]
	    constexpr bool isAbsoluteZero() const noexcept {
	        return kelvin_ == 0.0;
	    }

	    [[nodiscard]]
	    constexpr bool isAboveAbsoluteZero() const noexcept {
	        return kelvin_ > 0.0;
	    }

	public:

	    constexpr auto operator<=>(const Temperature&) const = default;

	public:

	    [[nodiscard]]
	    constexpr Temperature operator+(const TemperatureDifference difference) const noexcept;

	    [[nodiscard]]
	    constexpr Temperature operator-(const TemperatureDifference difference) const noexcept;

	    constexpr Temperature& operator+=(const TemperatureDifference difference) noexcept;

	    constexpr Temperature& operator-=(const TemperatureDifference difference) noexcept;

	public:

	    [[nodiscard]]
	    constexpr TemperatureDifference operator-(const Temperature rhs) const noexcept;

	private:

	    constexpr explicit Temperature(const ValueType kelvin) noexcept: kelvin_(kelvin)
	    {
	    }

	private:

	    ValueType kelvin_ {};
	};


	constexpr Temperature Temperature::operator+(const TemperatureDifference difference) const noexcept {
	    return fromKelvin(kelvin_ + difference.kelvin());
	}

	constexpr Temperature Temperature::operator-(const TemperatureDifference difference) const noexcept {
	    return fromKelvin(kelvin_ - difference.kelvin());
	}

	constexpr Temperature& Temperature::operator+=(const TemperatureDifference difference) noexcept {
	    kelvin_ += difference.kelvin();
	    return *this;
	}

	constexpr Temperature& Temperature::operator-=(const TemperatureDifference difference) noexcept
	{
	    kelvin_ -= difference.kelvin();
	    return *this;
	}

	constexpr TemperatureDifference Temperature::operator-(const Temperature rhs) const noexcept {
	    return TemperatureDifference::fromKelvin(kelvin_ - rhs.kelvin_);
	}
}

namespace units::literals
{

	[[nodiscard]]
	constexpr Temperature operator""_K(const long double value) noexcept {
	    return Temperature::fromKelvin(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr Temperature operator""_K(const unsigned long long value) noexcept {
	    return Temperature::fromKelvin(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr Temperature operator""_degC(const long double value) noexcept {
	    return Temperature::fromCelsius(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr Temperature operator""_degC(const unsigned long long value) noexcept {
	    return Temperature::fromCelsius(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr Temperature operator""_degF(const long double value) noexcept {
	    return Temperature::fromFahrenheit(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr Temperature operator""_degF(const unsigned long long value) noexcept {
	    return Temperature::fromFahrenheit(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr TemperatureDifference operator""_dK(const long double value) noexcept {
	    return TemperatureDifference::fromKelvin(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr TemperatureDifference operator""_dK(const unsigned long long value) noexcept {
	    return TemperatureDifference::fromKelvin(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr TemperatureDifference operator""_ddegC(const long double value) noexcept {
	    return TemperatureDifference::fromCelsius(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr TemperatureDifference operator""_ddegC(const unsigned long long value) noexcept {
	    return TemperatureDifference::fromCelsius(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr TemperatureDifference operator""_ddegF(const long double value) noexcept {
	    return TemperatureDifference::fromFahrenheit(static_cast<double>(value));
	}

	[[nodiscard]]
	constexpr TemperatureDifference operator""_ddegF(const unsigned long long value) noexcept {
	    return TemperatureDifference::fromFahrenheit(static_cast<double>(value));
	}
}

#endif //CPPFIRMWAREEMBEDDED_TEMPERATURE_HPP
