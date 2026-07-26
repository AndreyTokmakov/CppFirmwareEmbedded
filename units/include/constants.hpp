/**============================================================================
Name        : constants.hpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : constants.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_CONSTANTS_HPP
#define CPPFIRMWAREEMBEDDED_CONSTANTS_HPP

#pragma once

/**
 * ============================================================================
 * @file    constants.hpp
 * @brief   Fundamental mathematical and physical constants.
 *
 * ----------------------------------------------------------------------------
 * Overview
 * ----------------------------------------------------------------------------
 *
 * This file defines a collection of constexpr mathematical and physical
 * constants used throughout the units library.
 *
 * All constants are expressed in SI base units and are intended for firmware,
 * embedded software, digital signal processing (DSP), radio frequency (RF),
 * FPGA development and scientific applications.
 *
 * The constants are organized into a dedicated namespace to avoid polluting the
 * global namespace and to provide a single authoritative source for commonly
 * used values.
 *
 * ----------------------------------------------------------------------------
 * Usage
 * ----------------------------------------------------------------------------
 *
 * Example:
 *
 *      #include <units/constants.hpp>
 *
 *      double wavelength =
 *          units::constants::SPEED_OF_LIGHT / frequency.hz();
 *
 * ----------------------------------------------------------------------------
 * Design goals
 * ----------------------------------------------------------------------------
 *
 *  • constexpr-friendly
 *  • Header-only implementation
 *  • Strongly documented
 *  • SI units only
 *  • Zero runtime overhead
 *  • Suitable for firmware and embedded software
 *
 * ----------------------------------------------------------------------------
 * References
 * ----------------------------------------------------------------------------
 *
 *  • CODATA 2022 Recommended Values of the Fundamental Physical Constants
 *  • The International System of Units (SI Brochure, 9th Edition)
 *  • ISO 80000 — Quantities and Units
 *
 * ============================================================================
 */

namespace units::constants
{

    inline constexpr double PI = 3.141592653589793238462643383279502884;

    inline constexpr double TWO_PI = 2.0 * PI;

    inline constexpr double HALF_PI = PI / 2.0;

    inline constexpr double SQRT_2 = 1.414213562373095048801688724209698079;

    inline constexpr double E = 2.718281828459045235360287471352662498;

    //=============================================================================
    // Physical constants
    //=============================================================================

    /**
     * Speed of light in vacuum.
     *
     * Unit:
     *      m/s
     */
    inline constexpr double SPEED_OF_LIGHT = 299'792'458.0;

    /**
     * Standard acceleration due to gravity.
     *
     * Unit:
     *      m/s²
     */
    inline constexpr double STANDARD_GRAVITY = 9.80665;

    /**
     * Standard atmospheric pressure.
     *
     * Unit:
     *      Pa
     */
    inline constexpr double STANDARD_ATMOSPHERE = 101'325.0;

    /**
     * Absolute zero.
     *
     * Unit:
     *      °C
     */
    inline constexpr double ABSOLUTE_ZERO_CELSIUS = -273.15;

    /**
     * Absolute zero.
     *
     * Unit:
     *      K
     */
    inline constexpr double ABSOLUTE_ZERO_KELVIN = 0.0;

    //=============================================================================
    // Electromagnetic constants
    //=============================================================================

    /**
     * Characteristic impedance of free space.
     *
     * Unit:
     *      Ω
     */
    inline constexpr double FREE_SPACE_IMPEDANCE = 376.730313668;

    //=============================================================================
    // Time constants
    //=============================================================================

    inline constexpr double SECONDS_PER_MINUTE = 60.0;

    inline constexpr double MINUTES_PER_HOUR = 60.0;

    inline constexpr double HOURS_PER_DAY = 24.0;

    inline constexpr double SECONDS_PER_HOUR = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;

    inline constexpr double SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY;

} // namespace units::constants

#endif //CPPFIRMWAREEMBEDDED_CONSTANTS_HPP
