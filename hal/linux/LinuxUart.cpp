/**============================================================================
Name        : LinuxUart.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxUart.cpp
============================================================================**/

#include "LinuxUart.hpp"
#include <utility>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

/******************************************************************************
 *
 *  LinuxUart.cpp
 *
 *  Linux implementation of the HAL UART interface.
 *
 *  Overview
 *  --------
 *
 *  This module provides a Linux-specific implementation of the IUart
 *  abstraction using the POSIX terminal (TTY) API.
 *
 *  The purpose of this implementation is to expose a platform-independent
 *  UART interface while hiding Linux-specific details such as file
 *  descriptors, termios configuration and ioctl calls.
 *
 *  Higher-level firmware components should communicate only through the
 *  IUart interface and must never depend directly on Linux serial APIs.
 *
 *
 *  Typical Use Cases
 *  -----------------
 *
 *      • GPS receivers
 *      • Cellular modems
 *      • Bluetooth modules
 *      • LoRa transceivers
 *      • Debug console
 *      • MCU communication
 *      • FPGA communication
 *
 *
 *  Linux UART API
 *  --------------
 *
 *  This implementation uses:
 *
 *      • open()
 *      • read()
 *      • write()
 *      • termios
 *      • ioctl(FIONREAD)
 *      • tcflush()
 *
 *
 *  UART Configuration
 *  ------------------
 *
 *      • 8 data bits
 *      • No parity
 *      • One stop bit
 *      • No hardware flow control
 *      • Raw mode
 *
 *
 *  Thread Safety
 *  -------------
 *
 *  LinuxUart instances are not internally synchronized.
 *
 *  If multiple threads access the same UART simultaneously,
 *  synchronization must be provided by the caller.
 *
 *
 *  Resource Management
 *  -------------------
 *
 *  The UART device is opened during construction and automatically
 *  closed when the object is destroyed.
 *
 *
 *  Dependencies
 *  ------------
 *
 *      <fcntl.h>
 *      <unistd.h>
 *      <termios.h>
 *      <sys/ioctl.h>
 *
 *****************************************************************************/

namespace hal
{
    LinuxUart::LinuxUart(std::string  device, const uint32_t baudRate):
        devicePath(std::move(device)),
        baudRate(baudRate)
    {
        if (open() != Error::Success){
            return;
        }
        if (Error::Success == configure()) {
            throw std::runtime_error("Error in LinuxUart::configure()");
        }
    }


    LinuxUart::~LinuxUart()
    {
        if (fileDescriptor >= 0)
        {
            ::close(fileDescriptor);
            fileDescriptor = -1;
        }
    }


    Error LinuxUart::open()
    {
        fileDescriptor = ::open(devicePath.c_str(),O_RDWR | O_NOCTTY);
        if (fileDescriptor < 0){
            return Error::HardwareFailure;
        }
        return Error::Success;
    }


    Error LinuxUart::configure() const
    {
        if (fileDescriptor < 0) {
            return Error::HardwareFailure;
        }

        termios tty{};
        if (tcgetattr(fileDescriptor, &tty) != 0){
            return Error::HardwareFailure;
        }

        const speed_t speed = convertBaudRate(baudRate);

        cfsetispeed(&tty, speed);
        cfsetospeed(&tty, speed);

        tty.c_cflag |= CLOCAL;
        tty.c_cflag |= CREAD;
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_iflag = 0;
        tty.c_oflag = 0;
        tty.c_lflag = 0;
        tty.c_cc[VMIN] = 0;
        tty.c_cc[VTIME] = 0;

        if (tcsetattr(fileDescriptor, TCSANOW, &tty) != 0) {
            return Error::HardwareFailure;
        }
        return Error::Success;
    }


    Error LinuxUart::write(const uint8_t* data,
                           const uint32_t size)
    {
        if (fileDescriptor < 0)
        {
            return Error::HardwareFailure;
        }

        const ssize_t result = ::write(fileDescriptor, data, size);
        if (result < 0) {
            return Error::HardwareFailure;
        }

        if (static_cast<uint32_t>(result) != size) {
            return Error::Timeout;
        }

        return Error::Success;
    }


    Error LinuxUart::read(uint8_t* data,
                          const uint32_t size,
                          uint32_t& received)
    {
        received = 0;
        if (fileDescriptor < 0){
            return Error::HardwareFailure;
        }

        const ssize_t result = ::read(fileDescriptor, data, size);
        if (result < 0){
            return Error::HardwareFailure;
        }

        received = static_cast<uint32_t>(result);
        return Error::Success;
    }


    uint32_t LinuxUart::available() const
    {
        if (fileDescriptor < 0) {
            return 0;
        }

        int bytesAvailable = 0;
        if (0 != ::ioctl(fileDescriptor, FIONREAD, &bytesAvailable)){
            return 0;
        }

        return static_cast<uint32_t>(bytesAvailable);
    }

    Error LinuxUart::flush()
    {
        if (fileDescriptor < 0) {
            return Error::HardwareFailure;
        }

        if (0 != ::tcflush(fileDescriptor, TCIOFLUSH)){
            return Error::HardwareFailure;
        }

        return Error::Success;
    }

    Error LinuxUart::setBaudRate(const uint32_t value)
    {
        baudRate = value;
        return configure();
    }


    uint32_t LinuxUart::getBaudRate() const{
        return baudRate;
    }

    bool LinuxUart::isOpen() const noexcept {
        return fileDescriptor >= 0;
    }

    const std::string& LinuxUart::getDevicePath() const noexcept
    {
        return devicePath;
    }


    uint32_t LinuxUart::convertBaudRate(const uint32_t baudRate)
    {
        switch (baudRate)
        {
            case 9600:
                return B9600;
            case 19200:
                return B19200;
            case 38400:
                return B38400;
            case 57600:
                return B57600;
            case 115200:
                return B115200;
            case 230400:
                return B230400;
    #ifdef B460800
            case 460800:
                return B460800;
    #endif
    #ifdef B921600
            case 921600:
                return B921600;
    #endif
            default:
                return B115200;
        }
    }

} // namespace hal