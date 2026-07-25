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
     * This interface provides hardware-independent access to I2C communication.
     *
     * I2C is commonly used in embedded and RF systems for configuring
     * low-speed peripheral devices.
     *
     * Typical devices connected through I2C:
     *   - Temperature sensors
     *   - EEPROM memory
     *   - Clock generators
     *   - Power management ICs
     *   - RF monitoring devices
     *
     * The interface hides the underlying hardware implementation.
     *
     * Possible implementations:
     *   - Linux i2c-dev driver
     *   - STM32 HAL I2C
     *   - Zephyr I2C driver
     *   - FPGA I2C controller
     */
    struct II2cBus
    {
        virtual ~II2cBus() = default;

        /**
         * Writes data to an I2C slave device.
         * The transaction consists of:
         *   START
         *       |
         *   Slave address + WRITE
         *       |
         *   Data bytes
         *       |
         *   STOP
         *
         * Example:
         * Writing register 0x10:
         *   Address: 0x60
         *   Data:
         *       [0x10][0x55]
         *
         * @param address 7-bit I2C slave address.
         * @param data Buffer containing data to transmit.
         * @param size Number of bytes to transmit.
         * @return Operation status.
         */
        virtual Error write(uint8_t address,
                            const uint8_t* data,
                            uint32_t size) = 0;

        /**
         * Reads data from an I2C slave device.
         * The transaction consists of:
         *   START
         *       |
         *   Slave address + READ
         *       |
         *   Receive data
         *       |
         *   STOP
         *
         * @param address 7-bit I2C slave address.
         * @param data Buffer for received data.
         * @param size Number of bytes to receive.
         * @return Operation status.
         */
        virtual Error read(uint8_t address,
                           uint8_t* data,
                           uint32_t size) = 0;

        /**
         * Writes register address and then reads data.
         * Many embedded devices use a register-based protocol:
         * Example:
         *   Write:
         *       Register address = 0x20
         *   Read:
         *       Return register value
         *
         * This method performs a repeated-start transaction when supported.
         *
         * @param address 7-bit I2C slave address.
         * @param registerAddress Device register address.
         * @param data Buffer for received data.
         * @param size Number of bytes to receive.
         * @return Operation status.
         */
        virtual Error readRegister(uint8_t address,
                                   uint8_t registerAddress,
                                   uint8_t* data,
                                   uint32_t size) = 0;

        /**
         * Writes a value into a device register.
         * Commonly used by configuration drivers.
         * Example:
         *   PLL register 0x05 = 0x1234
         *
         * @param address 7-bit I2C slave address.
         * @param registerAddress Device register address.
         * @param value Value to write.
         * @return Operation status.
         */
        virtual Error writeRegister(uint8_t address,
                                    uint8_t registerAddress,
                                    uint8_t value) = 0;

        /**
         * Changes I2C clock frequency.
         *
         * Common frequencies:
         *   Standard mode:
         *       100 kHz
         *   Fast mode:
         *       400 kHz
         *   Fast mode plus:
         *       1 MHz
         *
         * @param frequencyHz Desired bus frequency.
         * @return Operation status.
         */
        virtual Error setFrequency(uint32_t frequencyHz) = 0;

        /**
         * Returns current I2C clock frequency.
         * @return Frequency in Hz.
         */
        [[nodiscard]]
        virtual uint32_t getFrequency() const = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_II2CBUS_HPP
