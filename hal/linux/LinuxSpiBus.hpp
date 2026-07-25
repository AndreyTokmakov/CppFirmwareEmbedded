/**============================================================================
Name        : LinuxSpiBus.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxSpiBus.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_LINUXSPIBUS_HPP
#define CPPFIRMWAREEMBEDDED_LINUXSPIBUS_HPP

#pragma once

#include <cstdint>
#include <string>

#include "../include/ISpiBus.hpp"
#include "Error.hpp"


namespace hal
{
    using common::Error;
    /**
     * Linux implementation of SPI bus using the spidev kernel interface.
     *
     * This class provides access to Linux SPI devices exposed through:
     *     /dev/spidevX.Y
     *
     * Example:
     *     /dev/spidev0.0
     *
     * The implementation uses the Linux kernel SPI userspace API.
     *
     * Supported configuration:
     *
     *   - SPI mode (0..3)
     *   - Clock frequency
     *   - Bits per word
     *   - Full duplex transfers
     *
     * This class is part of the HAL layer and does not contain anyknowledge about specific devices connected to SPI.
     *
     * Example usage:
     *     LinuxSpiBus spi( "/dev/spidev0.0", 1000000, 8);
     *     spi.Transfer(tx, rx, size);
     */
    class LinuxSpiBus : public ISpiBus
    {
    public:

        /**
         * Creates Linux SPI bus instance.
         * Opens the specified spidev device and configuresinitial SPI parameters.
         * @param device SPI device path.
         *
         * Example:
         *     /dev/spidev0.0
         *
         * @param frequencyHz SPI clock frequency.
         * @param mode SPI mode:
         *
         *     0 -> CPOL=0 CPHA=0
         *     1 -> CPOL=0 CPHA=1
         *     2 -> CPOL=1 CPHA=0
         *     3 -> CPOL=1 CPHA=1
         *
         * @param bitsPerWord Number of bits per SPI word.
         *
         * Usually:
         *     8 bits
         */
        explicit LinuxSpiBus(std::string  device,
                             uint32_t frequencyHz = 1000000,
                             uint8_t mode = 0,
                             uint8_t bitsPerWord = 8);

        /**
         * Closes SPI device handle.
         */
        ~LinuxSpiBus() override;

        /**
         * Performs SPI full-duplex transfer.
         * The operation is implemented using:
         *     SPI_IOC_MESSAGE
         *
         * ioctl request from Linux spidev API.
         *
         * @param txData Data to transmit.
         * @param rxData Buffer for received data.
         * @param size Number of bytes to transfer.
         * @return Operation status.
         */
        Error transfer(const uint8_t* txData,
                       uint8_t* rxData,
                       uint32_t size) override;

        /**
         * Changes SPI clock frequency.
         * The new value is applied immediately.
         * @param frequency New SPI frequency.
         * @return Operation status.
         */
        Error setFrequency(uint32_t frequency) override;

        /**
         * Returns current SPI clock frequency.
         * @return SPI frequency in Hz.
         */
        [[nodiscard]]
        uint32_t getFrequency() const noexcept override;

        /**
         * Changes SPI mode.
         * @param spiMode SPI mode:
         *     0..3
         *
         * @return Operation status.
         */
        Error setMode(uint8_t spiMode);

        /**
         * Returns current SPI mode.
         *
         * @return SPI mode.
         */
        [[nodiscard]]
        uint8_t getMode() const noexcept;

        /**
         * Changes number of bits transferred per word.
         * Typical values:
         *     8
         *     16
         *
         * @param bits Number of bits.
         * @return Operation status.
         */
        Error setBitsPerWord(uint8_t bits);

        /**
         * Returns current bits per word configuration.
         * @return Number of bits.
         */
        [[nodiscard]]
        uint8_t getBitsPerWord() const noexcept;

        /**
         * Returns true if SPI device was successfully opened.
         *
         * @return Device state.
         */
        [[nodiscard]]
        bool isOpen() const noexcept;

    private:

        /**
         * Opens Linux spidev device.
         */
        Error open();

        /**
         * Applies current configuration to Linux SPI driver.
         */
        Error configure();

    private:

        int fileDescriptor = -1;
        std::string devicePath;
        uint32_t frequencyHz = 0;
        uint8_t mode = 0;
        uint8_t bitsPerWord = 8;
    };
}

#endif //CPPFIRMWAREEMBEDDED_LINUXSPIBUS_HPP
