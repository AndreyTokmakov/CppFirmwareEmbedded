/**============================================================================
Name        : IPwm.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IPwm.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IPWM_HPP
#define CPPFIRMWAREEMBEDDED_IPWM_HPP

#pragma once
#include <cstdint>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract PWM interface.
     *
     * Provides hardware-independent pulse width modulation control.
     *
     * PWM is commonly used in embedded RF systems for:
     *
     *   - PA enable control
     *   - Power regulation
     *   - Bias voltage generation
     *   - RF switch timing
     *   - Motor/fan control
     *
     * Possible implementations:
     *
     *   - MCU timer peripherals
     *   - FPGA PWM blocks
     *   - Linux PWM subsystem
     */
    struct IPwm
    {
        virtual ~IPwm() = default;

        /**
         * Enables PWM output.
         * @return Operation status.
         */
        virtual Error enable() = 0;

        /**
         * Disables PWM output.
         * @return Operation status.
         */
        virtual Error disable() = 0;

        /**
         * Sets PWM frequency.
         * Example:
         *   1 kHz PWM
         *
         * @param frequencyHz PWM frequency.
         * @return Operation status.
         */
        virtual Error setFrequency(uint32_t frequencyHz) = 0;

        /**
         * Sets PWM duty cycle.
         * Duty cycle range:
         *   0.0:
         *       always LOW
         *   1.0:
         *       always HIGH
         * Example:
         *   0.5:
         *       50% duty cycle
         * @param dutyCycle Value from 0.0 to 1.0.
         * @return Operation status.
         */
        virtual Error setDutyCycle(double dutyCycle) = 0;

        /**
         * Returns current PWM frequency.
         * @return Frequency in Hz.
         */
        [[nodiscard]]
        virtual uint32_t getFrequency() const = 0;

        /**
         * Returns current duty cycle.
         * @return Duty cycle from 0.0 to 1.0.
         */
        [[nodiscard]]
        virtual double getDutyCycle() const = 0;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_IPWM_HPP
