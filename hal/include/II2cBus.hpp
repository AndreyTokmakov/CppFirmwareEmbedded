/**============================================================================
Name        : II2cBus.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : II2cBus.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_II2CBUS_HPP
#define CPPFIRMWAREEMBEDDED_II2CBUS_HPP

#include <cstdint>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract I2C bus interface.
     *
     * Provides a platform-independent interface for communicating with
     * devices connected to an Inter-Integrated Circuit (I2C) bus.
     *
     * This interface represents the transport layer only and intentionally
     * contains no knowledge about the protocol implemented by individual
     * devices connected to the bus.
     *
     * Typical devices connected over I2C include:
     *     • EEPROM memories
     *     • Temperature sensors
     *     • Real-Time Clocks (RTC)
     *     • GPIO expanders
     *     • Power monitors
     *     • ADCs
     *     • DACs
     *     • Clock generators
     *     • PMICs
     *     • RF front-end devices
     *
     * Implementations may use:
     *     • Linux i2c-dev
     *     • STM32 HAL
     *     • Zephyr drivers
     *     • Bare-metal drivers
     *     • FPGA controllers
     *     • USB-to-I2C adapters
     *
     * The interface supports:
     *     • Write transactions
     *     • Read transactions
     *     • Combined write/read transactions using a repeated START
     */
    struct II2cBus
    {
        virtual ~II2cBus() = default;

        /**
         * Writes data to an I2C slave device.
         * A complete I2C write transaction is performed.
         *
         * Typical transaction:
         *
         *     START
         *     Address + Write
         *     Data...
         *     STOP
         *
         * @param address 7-bit I2C slave address.
         * @param data Pointer to the transmit buffer.
         * @param size Number of bytes to transmit.
         * @return Operation status.
         */
        virtual Error write(uint8_t address,
                            const uint8_t* data,
                            uint32_t size) = 0;

        /**
         * Reads data from an I2C slave device.
         * A complete I2C read transaction is performed.
         *
         * Typical transaction:
         *     START
         *     Address + Read
         *     Data...
         *     STOP
         *
         * @param address 7-bit I2C slave address.
         * @param data Pointer to the receive buffer.
         * @param size Number of bytes to read.
         * @return Operation status.
         */
        virtual Error read(uint8_t address,
                           uint8_t* data,
                           uint32_t size) = 0;

        /**
         * Performs a combined write/read transaction without releasing the
         * I2C bus between the write and read phases.
         *
         * This operation uses a repeated START condition and is commonly
         * used to access registers inside I2C devices.
         *
         * Typical transaction:
         *     START
         *     Address + Write
         *     Write data...
         *     REPEATED START
         *     Address + Read
         *     Read data...
         *     STOP
         *
         * The transmitted data may contain:
         *     • Register address
         *     • Command
         *     • Memory address
         *     • Device-specific request
         *
         * @param address 7-bit I2C slave address.
         * @param txData Pointer to transmit buffer.
         * @param txSize Number of bytes to transmit.
         * @param rxData Pointer to receive buffer.
         * @param rxSize Number of bytes to receive.
         * @return Operation status.
         */
        virtual Error transfer(uint8_t address,
                               const uint8_t* txData,
                               uint32_t txSize,
                               uint8_t* rxData,
                               uint32_t rxSize) = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_II2CBUS_HPP
