/**============================================================================
Name        : IAdc.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IAdc.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IADC_HPP
#define CPPFIRMWAREEMBEDDED_IADC_HPP

#include <cstdint>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract ADC interface.
     *
     * Provides hardware-independent access to analog-to-digital conversion functionality.
     *
     * ADCs are commonly used in RF and embedded systems for:
     *   - RSSI measurements
     *   - RF power monitoring
     *   - Temperature monitoring
     *   - Analog feedback loops
     *   - Sensor acquisition
     *
     * The interface hides ADC hardware details:
     *   - MCU internal ADC
     *   - External SPI ADC
     *   - FPGA ADC interface
     *   - High-speed RF ADC
     *
     * The implementation is responsible for:
     *   - sampling configuration
     *   - reference voltage handling
     *   - conversion timing
     *   - calibration
     */
    struct IAdc
    {
        virtual ~IAdc() = default;

        /**
         * Reads a single ADC conversion result.
         * The returned value is the raw ADC code.
         *
         * Example:
         *   12-bit ADC:
         *       Minimum:
         *           0
         *       Maximum:
         *           4095
         *
         * @param channel ADC input channel.
         * @param value Output ADC raw value.
         * @return Operation status.
         */
        virtual Error read(uint8_t channel,
                           uint16_t& value) = 0;

        /**
         * Converts raw ADC value into voltage.
         *
         * The conversion depends on:
         *   - ADC resolution
         *   - reference voltage
         *   - calibration parameters
         *
         * Example:
         *   ADC = 2048
         *   Vref = 3.3V
         *   Result:
         *       approximately 1.65V
         *
         * @param channel ADC input channel.
         * @param voltage Output voltage value.
         * @return Operation status.
         */
        virtual Error readVoltage(uint8_t channel,
                                  double& voltage) = 0;

        /**
         * Returns ADC resolution.
         *
         * Example:
         *   12-bit ADC:
         *       4096 possible values
         * @return Resolution in bits.
         */
        [[nodiscard]]
        virtual uint8_t getResolution() const = 0;

        /**
         * Returns ADC reference voltage.
         * @return Reference voltage in volts.
         */
        [[nodiscard]]
        virtual double getReferenceVoltage() const = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_IADC_HPP
