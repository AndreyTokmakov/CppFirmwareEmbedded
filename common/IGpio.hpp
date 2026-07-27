/**============================================================================
Name        : IGpio.hpp
Created on  : 27.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IGpio.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IGPIO_HPP
#define CPPFIRMWAREEMBEDDED_IGPIO_HPP

#pragma once

#include <cstdint>
#include <iostream>
#include <unordered_map>

#include "Error.hpp"

namespace hal
{
    using common::Error;


    /**
     * GPIO direction.
     *
     * Defines whether a GPIO pin is controlled by the software or by external
     * hardware.
     */
    enum class GpioDirection : uint8_t
    {
        Input,
        Output
    };


    /**
     * GPIO logical level.
     */
    enum class GpioLevel : uint8_t
    {
        Low = 0,
        High = 1
    };


    /**
     * Abstract GPIO interface.
     *
     * Overview
     * --------
     *
     * Provides hardware-independent access to digital input/output pins.
     *
     * Typical implementations:
     *
     *     • Linux GPIO character device (libgpiod)
     *     • MCU GPIO peripheral
     *     • FPGA GPIO controller
     *     • Mock GPIO for unit tests
     *
     * GPIO is commonly used in embedded RF systems for:
     *
     *     • RF switches
     *     • Power amplifier enable signals
     *     • LNA enable control
     *     • Digital attenuator control
     *     • Reset lines
     *     • Chip select signals
     *     • Latch enable signals
     *
     * Responsibilities
     * ----------------
     *
     * This interface provides:
     *
     *     • pin direction control
     *     • digital output control
     *     • digital input reading
     *
     * Device drivers should not depend on a specific GPIO implementation.
     */
    class IGpio
    {
    public:

        virtual ~IGpio() = default;

        /**
         * Configures GPIO direction.
         *
         * @param pin GPIO pin identifier.
         * @param direction Desired pin direction.
         *
         * @return Operation status.
         */
        virtual Error setDirection(uint32_t pin, GpioDirection direction) = 0;


        /**
         * Writes logical level to GPIO output.
         *
         * The pin must be configured as Output before calling this method.
         *
         * @param pin GPIO pin identifier.
         * @param level Output level.
         *
         * @return Operation status.
         */
        virtual Error write(uint32_t pin, GpioLevel level) = 0;

        /**
         * Reads GPIO input level.
         *
         * @param pin GPIO pin identifier.
         * @param level Returned GPIO level.
         *
         * @return Operation status.
         */
        virtual Error read(uint32_t pin, GpioLevel& level) const = 0;

        /**
         * Changes GPIO output state.
         *
         * Convenience helper for boolean values.
         *
         * @param pin GPIO pin identifier.
         * @param state Output state.
         *
         * @return Operation status.
         */
        Error write(const uint32_t pin, const bool state)
        {
            return write(pin, state ? GpioLevel::High : GpioLevel::Low);
        }
    };


    /**
     * Header-only GPIO mock implementation.
     *
     * Used for:
     *
     *     • unit tests
     *     • driver development
     *     • hardware simulation
     *
     * This implementation does not access real hardware.
     */
    struct  MockGpio final : public IGpio
    {
        Error setDirection(const uint32_t pin, const GpioDirection direction) override
        {
            directions[pin] = direction;
            return Error::Success;
        }

        Error write(const uint32_t pin, const GpioLevel level) override
        {
            levels[pin] = level;
            std::cout << "GPIO write pin=" << pin<< " level=" << (level == GpioLevel::High ? "HIGH" : "LOW") << '\n';
            return Error::Success;
        }

        Error read(const uint32_t pin, GpioLevel& level) const override
        {
            const auto iterator = levels.find(pin);
            if (iterator == levels.end()){
                return Error::ResourceUnavailable;
            }

            level = iterator->second;
            return Error::Success;
        }

        [[nodiscard]]
        GpioLevel getLevel(const uint32_t pin) const
        {
            const auto iterator = levels.find(pin);
            if (iterator == levels.end()) {
                return GpioLevel::Low;
            }

            return iterator->second;
        }

    private:

        std::unordered_map<uint32_t, GpioDirection> directions;
        std::unordered_map<uint32_t, GpioLevel> levels;
    };
}

#endif //CPPFIRMWAREEMBEDDED_IGPIO_HPP
