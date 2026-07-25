/**============================================================================
Name        : LinuxGpio.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxGpio.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_LINUXGPIO_HPP
#define CPPFIRMWAREEMBEDDED_LINUXGPIO_HPP

#include "../include/IGpio.hpp"
#include "Error.hpp"

struct gpiod_chip;
struct gpiod_line;

namespace hal
{   /**
     * Linux implementation of the GPIO interface using libgpiod.
     * This class provides access to GPIO lines exposed through the Linux GPIO character device interface.
     *
     * Example GPIO chips:
     *     /dev/gpiochip0
     *     /dev/gpiochip1
     *
     * Typical RF applications:
     *   - PE4312 Latch Enable
     *   - Chip Select
     *   - Device Reset
     *   - RF Enable
     *   - Power Down
     *
     * The GPIO line is requested exclusively while this object exists.
     **/
    struct LinuxGpio : public IGpio
    {   /**
         * Opens a GPIO line.
         * @param chipName GPIO chip name.
         *
         * Example:
         *     "gpiochip0"
         *
         * @param lineNumber GPIO line offset inside the chip.
         */
        LinuxGpio(const std::string& chipName, uint16_t lineNumber);

        /**
         * Releases the GPIO line.
         */
        ~LinuxGpio() override;

        /**
         * Sets GPIO output level.
         * The pin must previously be configured as an output.
         *
         * @param high true for HIGH level, false for LOW level.
         * @return Operation status.
         */
        Error set(bool high) override;

        /**
         * Returns the current GPIO level.
         * @return Current logic level.
         */
        [[nodiscard]]
        bool get() const override;

        /**
         * Configures GPIO as an output.
         *
         * @param initialState Initial output state.
         * @return Operation status.
         */
        Error configureOutput(bool initialState) override;

        /**
         * Configures GPIO as an input.
         *
         * @return Operation status.
         */
        Error configureInput() override;

        /**
         * Returns true if the GPIO line has been successfully opened.
         *
         * @return GPIO state.
         */
        [[nodiscard]]
        bool isOpen() const noexcept;

    private:

        gpiod_chip* chip = nullptr;
        gpiod_line* line = nullptr;
        bool state = false;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_LINUXGPIO_HPP
