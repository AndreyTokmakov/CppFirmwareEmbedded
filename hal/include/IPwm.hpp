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

#include <cstdint>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract Pulse Width Modulation (PWM) interface.
     * Provides a platform-independent abstraction for generating PWM signals.
     *
     * A PWM (Pulse Width Modulation) output is a digital signal whose average value is controlled
     * by adjusting the duty cycle while maintaining a fixed switching frequency.
     *
     * This interface represents a single PWM output channel rather than a specific hardware device.
     * Depending on the target platform, a PWM channel may be implemented by:
     *
     *   • A microcontroller timer peripheral
     *   • An FPGA PWM generator
     *   • The Linux PWM subsystem
     *   • An external PWM controller connected via SPI or I2C
     *
     * The implementation is responsible for configuring the underlying hardware and generating the requested PWM waveform.
     * Higher-level software should communicate only through this interface and must not depend on platform-specific APIs.
     *
     * Typical applications include:
     *
     *   • Power amplifier (PA) bias control
     *   • RF front-end enable timing
     *   • RF switch control
     *   • Variable power regulation
     *   • Bias voltage generation using an RC low-pass filter
     *   • Fan or motor speed control
     *   • LED brightness control
     *   • General-purpose timing signals
     *
     * Typical implementations:
     *
     *   • Stm32Pwm
     *   • LinuxPwm
     *   • FpgaPwm
     *   • Pca9685Pwm
     *
     * PWM itself is not a device. It is a hardware resource that can be used by other device drivers. For example:
     *
     *   • A fan controller may use PWM to regulate motor speed.
     *   • An LED driver may use PWM to control brightness.
     *   • An RF power amplifier may use PWM and an RC filter to generate
     *     an analog bias voltage.
     *   • An RF switch controller may use PWM-generated timing signals.
     *
     * This interface intentionally exposes only generic PWM functionality.
     * Device-specific behavior belongs to higher-level drivers rather than to the HAL layer.
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
        virtual uint32_t getFrequency() const noexcept = 0;

        /**
         * Returns current duty cycle.
         * @return Duty cycle from 0.0 to 1.0.
         */
        [[nodiscard]]
        virtual double getDutyCycle() const noexcept = 0;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_IPWM_HPP
