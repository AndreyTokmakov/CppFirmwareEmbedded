/**============================================================================
Name        : LinuxUart.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxUart.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_LINUXUART_HPP
#define CPPFIRMWAREEMBEDDED_LINUXUART_HPP

#include <cstdint>
#include <string>

#include "../include/IUart.hpp"
#include "Error.hpp"

namespace hal
{   /**
     * Linux implementation of the UART interface.
     * This class provides access to serial ports exposed by the Linux TTY subsystem.
     * The implementation uses the POSIX terminal API (termios) and provides a platform-independent
     * UART abstraction for higher-level firmware libraries.
     *
     * Typical serial devices:
     *     /dev/ttyS0
     *     /dev/ttyS1
     *     /dev/ttyUSB0
     *     /dev/ttyUSB1
     *     /dev/ttyACM0
     *
     * Typical use cases:
     *     • GPS receivers
     *     • Cellular modems
     *     • Bluetooth modules
     *     • LoRa transceivers
     *     • Debug console
     *     • MCU communication
     *     • FPGA communication
     *
     * This class is responsible only for UART transport.
     * It has no knowledge of any communication protocol running over the serial interface.
     * The UART device is opened during construction and automatically closed when the object is destroyed.
     */
    class LinuxUart : public IUart
    {
    public:

        /**
         * Opens a Linux UART device.
         * Example device names:
         *
         *     "/dev/ttyUSB0"
         *     "/dev/ttyACM0"
         *     "/dev/ttyS0"
         *
         * @param device UART device path.
         * @param baudRate Initial baud rate.
         */
        explicit LinuxUart(std::string  device,
                           uint32_t baudRate = 115200);

        /**
         * Closes the UART device.
         */
        ~LinuxUart() override;

        /**
         * Sends data through the serial port.
         * The function blocks until the data has been accepted by the operating system.
         *
         * @param data Pointer to transmit buffer.
         * @param size Number of bytes to transmit.
         * @return Operation status.
         */
        Error write(const uint8_t* data, uint32_t size) override;

        /**
         * Reads available data from the serial port.
         * The function returns immediately after reading the available
         * data or reaching the requested buffer size.
         *
         * @param data Receive buffer.
         * @param size Maximum number of bytes to read.
         * @param received Number of bytes actually received.
         * @return Operation status.
         */
        Error read(uint8_t* data,
                   uint32_t size,
                   uint32_t& received) override;

        /**
         * Returns the number of bytes currently available for reading.
         * This function allows non-blocking polling of the receive buffer.
         *
         * @return Number of available bytes.
         */
        [[nodiscard]]
        uint32_t available() const override;

        /**
         * Flushes both transmit and receive buffers.
         * Any pending transmit data is discarded and unread receive data is removed.
         *
         * @return Operation status.
         */
        Error flush() override;

        /**
         * Changes UART baud rate.
         * The new configuration is applied immediately.
         *
         * Common baud rates:
         *     9600
         *     19200
         *     38400
         *     57600
         *     115200
         *     230400
         *     460800
         *     921600
         *
         * @param baudRate Desired baud rate.
         * @return Operation status.
         */
        Error setBaudRate(uint32_t baudRate) override;

        /**
         * Returns the current UART baud rate.
         *
         * @return Configured baud rate.
         */
        [[nodiscard]]
        uint32_t getBaudRate() const override;

        /**
         * Returns true if the UART device has been successfully opened.
         *
         * @return Device state.
         */
        [[nodiscard]]
        bool isOpen() const noexcept;

        /**
         * Returns the Linux device path.
         *
         * Example:
         *     "/dev/ttyUSB0"
         *
         * @return Device path.
         */
        [[nodiscard]]
        const std::string& getDevicePath() const noexcept;

    private:
        /**
         * Opens the Linux TTY device.
         *
         * @return Operation status.
         */
        Error open();

        /**
         * Applies the current UART configuration.
         *
         * This method configures:
         *     • baud rate
         *     • 8 data bits
         *     • no parity
         *     • one stop bit
         *     • no hardware flow control
         *     • raw mode
         *
         * @return Operation status.
         */
        Error configure() const;

        /**
         * Converts a numeric baud rate into the corresponding POSIX termios constant.
         *
         * @param baudRate UART baud rate.
         * @return POSIX speed_t constant.
         */
        [[nodiscard]]
        static uint32_t convertBaudRate(uint32_t baudRate);

    private:

        int fileDescriptor = -1;
        std::string devicePath;
        uint32_t baudRate = 115200;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_LINUXUART_HPP
