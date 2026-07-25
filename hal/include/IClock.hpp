/**============================================================================
Name        : IClock.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IClock.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_ICLOCK_HPP
#define CPPFIRMWAREEMBEDDED_ICLOCK_HPP

#include <cstdint>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract clock source interface.
     * Represents a hardware clock source used by RF components.
     *
     * Clock sources are critical in RF systems because frequency accuracy,
     * phase noise and synchronization depend on reference clock quality.
     *
     * Typical clock sources:
     *   - TCXO
     *   - OCXO
     *   - Crystal oscillator
     *   - PLL synthesizer
     *   - Clock generator IC
     *
     * Examples of devices depending on this interface:
     *   - AD9361
     *   - ADF4351
     *   - ADF5355
     *   - RF ADC/DAC converters
     */
    struct IClock
    {
        virtual ~IClock() = default;

        /**
         * Enables the clock output.
         * Some clock generators require explicit enable control.
         * @return Operation status.
         */
        virtual Error enable() = 0;

        /**
         * Disables the clock output.
         * @return Operation status.
         */
        virtual Error disable() = 0;

        /**
         * Returns current output frequency.
         * @return Frequency in Hz.
         */
        [[nodiscard]]
        virtual uint64_t getFrequency() const = 0;

        /**
         * Changes output frequency.
         * The implementation may reconfigure:
         *   - PLL dividers
         *   - fractional synthesizer settings
         *   - clock generator registers
         *
         * @param frequencyHz Desired frequency.
         * @return Operation status.
         */
        virtual Error setFrequency(uint64_t frequencyHz) = 0;

        /**
         * Checks whether the clock is locked.
         * For PLL based clocks this indicates whether the internal
         * feedback loop has reached a stable frequency.
         *
         * @return true if clock is locked.
         */
        [[nodiscard]]
        virtual bool isLocked() const = 0;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_ICLOCK_HPP
