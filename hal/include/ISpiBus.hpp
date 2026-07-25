/**============================================================================
Name        : ISpiBus.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : ISpiBus.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_ISPIBUS_HPP
#define CPPFIRMWAREEMBEDDED_ISPIBUS_HPP

#include <cstdint>

#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract SPI bus interface.
     *
     * This interface provides a hardware-independent API for SPI
     * communication.
     *
     * RF device drivers should depend only on this interface and must not
     * contain any platform-specific SPI implementation details.
     *
     * Possible implementations:
     *   - Linux spidev
     *   - STM32 HAL SPI
     *   - Zephyr SPI driver
     *   - FPGA SPI controller
     *   - Bare-metal MCU driver
     *
     * The interface represents a physical SPI controller.
     * Chip-select handling may be implemented either inside the SPI driver
     * or externally using a dedicated GPIO interface.
     */
    struct ISpiBus
    {
        virtual ~ISpiBus() = default;

        /**
         * Performs a full-duplex SPI transaction.
         *
         * SPI communication simultaneously transmits and receives data.
         * The function transfers exactly 'size' bytes.
         * Example:
         *   TX:
         *       [0x12][0x34]
         *   RX:
         *       [0x00][0x00]
         *
         * @param txData Pointer to transmit buffer.
         *               Can be nullptr for read-only transfers.
         * @param rxData Pointer to receive buffer.
         *               Can be nullptr for write-only transfers.
         * @param size Number of bytes to transfer.
         *
         * @return Operation result.
         */
        virtual Error transfer(const uint8_t* txData,
                               uint8_t* rxData,
                               uint32_t size) = 0;

        /**
         * Changes SPI clock frequency.
         *
         * Some RF devices require different SPI speeds depending on operating mode or initialization stage.
         *
         * Example:
         *   Initialization:
         *       1 MHz
         *   Normal operation:
         *       20 MHz
         * @param frequencyHz Desired SPI clock frequency.
         *
         * @return Operation result.
         */
        virtual Error setFrequency(uint32_t frequencyHz) = 0;

        /**
         * Returns current SPI clock frequency.
         *
         * @return SPI frequency in Hz.
         */
        [[nodiscard]]
        virtual uint32_t getFrequency() const = 0;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_ISPIBUS_HPP
