/**============================================================================
Name        : IDac.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IDac.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IDAC_HPP
#define CPPFIRMWAREEMBEDDED_IDAC_HPP


#include <cstdint>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract DAC interface.
     * Provides hardware-independent digital-to-analog conversion.
     *
     * DACs are frequently used in RF systems for:
     *   - PA bias control
     *   - LNA control
     *   - VGA gain control
     *   - PLL tuning voltage
     *   - Analog calibration
     *
     * Possible implementations:
     *   - MCU internal DAC
     *   - SPI DAC
     *   - FPGA DAC controller
     */
    struct IDac
    {
        virtual ~IDac() = default;

        /**
         * Writes a raw DAC value.
         * Example:
         *   12-bit DAC:
         *       Range:
         *           0 ... 4095
         *
         * @param channel DAC output channel.
         * @param value Raw DAC code.
         * @return Operation status.
         */
        virtual Error write(uint8_t channel,
                           uint16_t value) = 0;

        /**
         * Sets DAC output voltage.
         *
         * The implementation converts voltage into the appropriate DAC code using calibration parameters.
         *
         * @param channel DAC output channel.
         * @param voltage Desired output voltage.
         * @return Operation status.
         */
        virtual Error writeVoltage(uint8_t channel,
                                   double voltage) = 0;

        /**
         * Returns DAC resolution.
         * @return Resolution in bits.
         */
        [[nodiscard]]
        virtual uint8_t getResolution() const = 0;

        /**
         * Returns DAC reference voltage.
         * @return Reference voltage in volts.
         */
        [[nodiscard]]
        virtual double getReferenceVoltage() const = 0;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_IDAC_HPP
