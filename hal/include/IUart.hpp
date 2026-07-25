/**============================================================================
Name        : IUart.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IUart.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IUART_HPP
#define CPPFIRMWAREEMBEDDED_IUART_HPP

#include <cstdint>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract UART interface.
     * Provides hardware-independent serial communication.
     *
     * UART is commonly used in embedded systems for:
     *   - GPS receivers
     *   - External RF modules
     *   - Modems
     *   - Debug consoles
     *   - MCU-to-MCU communication
     *
     * The interface does not define:
     *   - physical voltage levels
     *   - RS232 / RS485 transceivers
     *   - DMA usage
     *   - interrupt handling
     *
     * These details belong to the platform implementation.
     *
     * Possible implementations:
     *   - Linux tty driver
     *   - STM32 USART
     *   - FPGA UART core
     */
    struct IUart
    {
        virtual ~IUart() = default;

        /**
         * Sends data through UART.
         * The function blocks until data is accepted by the underlying UART implementation.
         *
         * @param data Buffer to transmit.
         * @param size Number of bytes to transmit.
         * @return Operation status.
         */
        virtual Error write(const uint8_t* data,
                            uint32_t size) = 0;

        /**
         * Reads received UART data.
         * The function returns immediately with the currently available amount of data.
         *
         * @param data Buffer for received data.
         * @param size Maximum number of bytes to read.
         * @param received Actual number of bytes received.
         * @return Operation status.
         */
        virtual Error read(uint8_t* data,
                           uint32_t size,
                           uint32_t& received) = 0;

        /**
         * Returns the number of bytes currently available for reading.
         * This method allows non-blocking communication.
         * Example:
         *   if (uart.Available() > 0)
         *   {
         *       uart.Read(...);
         *   }
         * @return Number of available bytes.
         */
        [[nodiscard]]
        virtual uint32_t available() const = 0;

        /**
         * Clears internal receive and transmit buffers.
         * Useful after device reset or communication errors.
         * @return Operation status.
         */
        virtual Error flush() = 0;

        /**
         * Changes UART baud rate.
         * Common baud rates:
         *   - 9600
         *   - 115200
         *   - 921600
         *
         * @param baudRate Desired baud rate.
         * @return Operation status.
         */
        virtual Error setBaudRate(uint32_t baudRate) = 0;

        /**
         * Returns current UART baud rate.
         * @return Baud rate.
         */
        [[nodiscard]]
        virtual uint32_t getBaudRate() const = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_IUART_HPP
