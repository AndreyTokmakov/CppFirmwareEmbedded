/**============================================================================
Name        : IGpio.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IGpio.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IGPIO_HPP
#define CPPFIRMWAREEMBEDDED_IGPIO_HPP

#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract GPIO interface.
     * Provides access to digital input/output pins.
     *
     * RF devices frequently require GPIO control signals:
     *   - Latch Enable (LE)
     *   - Chip Select (CS)
     *   - Reset
     *   - Enable
     *   - Power-down
     *   - Synchronization signals
     *
     * This interface hides the underlying GPIO implementation.
     *
     * Possible implementations:
     *   - Linux GPIO character device
     *   - STM32 GPIO peripheral
     *   - FPGA GPIO registers
     */
    struct IGpio
    {
        virtual ~IGpio() = default;

        /**
         * Sets GPIO output state.
         * The pin must be configured as an output.
         *
         * Example:
         *   Set(true);
         *   GPIO voltage:
         *       HIGH
         *   Set(false);
         *   GPIO voltage:
         *       LOW
         *
         * @param high Desired output level.
         *
         * @return Operation result.
         */
        virtual Error set(bool high) = 0;

        /**
         * Returns current GPIO output state.
         * For output pins this normally returns the last written state.
         * For input pins this returns the actual hardware level.
         * @return Current GPIO level.
         */
        [[nodiscard]]
        virtual bool get() const = 0;

        /**
         * Configures GPIO as output.
         * The initial state is applied immediately after configuration.
         * @param initialState Initial output level.
         * @return Operation result.
         */
        virtual Error configureOutput(bool initialState) = 0;

        /**
         * Configures GPIO as input.
         * Input configuration is required for reading external signals.
         * @return Operation result.
         */
        virtual Error configureInput() = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_IGPIO_HPP
