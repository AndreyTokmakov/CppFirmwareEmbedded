/**============================================================================
Name        : LinuxI2cBus.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxI2cBus.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_LINUXI2CBUS_HPP
#define CPPFIRMWAREEMBEDDED_LINUXI2CBUS_HPP


#include <cstdint>
#include <string>

#include "../include/II2cBus.hpp"

namespace hal
{

    /**
     * Linux implementation of the I2C bus interface.
     *
     * This class provides access to Linux I2C adapters exposed through the
     * i2c-dev kernel driver.
     *
     * Typical Linux devices:
     *
     *     /dev/i2c-0
     *     /dev/i2c-1
     *     /dev/i2c-2
     *
     * The implementation hides Linux-specific details and exposes a
     * platform-independent I2C interface suitable for firmware libraries.
     *
     * Typical devices connected over I2C include:
     *
     *     • Temperature sensors
     *     • EEPROMs
     *     • GPIO expanders
     *     • Power monitors
     *     • RTC devices
     *     • Clock generators
     *     • ADCs
     *     • DACs
     *     • PMICs
     *
     * This class represents the I2C bus only.
     * It contains no knowledge of the devices connected to the bus.
     *
     * The device is opened during construction and automatically closed
     * when the object is destroyed.
     */
    class LinuxI2cBus : public II2cBus
    {
    public:

        /**
         * Opens a Linux I2C bus.
         *
         * Example:
         *     "/dev/i2c-1"
         * @param device Linux I2C device path.
         */
        explicit LinuxI2cBus(std::string  device);

        /**
         * Closes the I2C device.
         */
        ~LinuxI2cBus() override;

        /**
         * Writes data to an I2C slave.
         *
         * @param address 7-bit slave address.
         * @param data Data to transmit.
         * @param size Number of bytes.
         * @return Operation status.
         */
        Error write(uint8_t address,
                    const uint8_t* data,
                    uint32_t size) override;

        /**
         * Reads data from an I2C slave.
         *
         * @param address 7-bit slave address.
         * @param data Receive buffer.
         * @param size Number of bytes.
         * @return Operation status.
         */
        Error read(uint8_t address,
                   uint8_t* data,
                   uint32_t size) override;

        /**
         * Performs a combined write followed immediately by a read without releasing the I2C bus.
         * This transaction is commonly used to access internal registers of I2C devices.
         *
         * @param address 7-bit slave address.
         * @param txData Data written before the read operation.
         * @param txSize Number of bytes to write.
         * @param rxData Receive buffer.
         * @param rxSize Number of bytes to read.
         * @return Operation status.
         */
        Error transfer(uint8_t address,
                       const uint8_t* txData,
                       uint32_t txSize,
                       uint8_t* rxData,
                       uint32_t rxSize) override;

        /**
         * Returns true if the I2C device has been successfully opened.
         *
         * @return Bus state.
         */
        [[nodiscard]]
        bool isOpen() const noexcept;


        /**
         * Returns the Linux device path.
         *
         * Example:
         *     "/dev/i2c-1"
         * @return Device path.
         */
        [[nodiscard]]
        const std::string& getDevicePath() const noexcept;

    private:

        /**
         * Opens the Linux I2C device.
         *
         * @return Operation status.
         */
        Error open();

        /**
         * Selects the active slave address.
         *
         * @param address 7-bit slave address.
         * @return Operation status.
         */
        Error selectDevice(uint8_t address);

    private:

        int fileDescriptor = -1;
        std::string devicePath;
        uint8_t currentAddress = 0xFF;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_LINUXI2CBUS_HPP
