/**============================================================================
Name        : LinuxI2cBus.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxI2cBus.cpp
============================================================================**/

#include "LinuxI2cBus.hpp"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <utility>

/******************************************************************************
 *
 *  LinuxI2cBus.cpp
 *
 *  Linux implementation of the HAL I2C interface.
 *
 *  Overview
 *  --------
 *
 *  This module implements the II2cBus interface using the Linux i2c-dev
 *  userspace API.
 *
 *  The implementation provides a platform-independent I2C abstraction
 *  while hiding Linux-specific concepts such as file descriptors and
 *  ioctl() requests.
 *
 *  Typical Linux devices:
 *
 *      /dev/i2c-0
 *      /dev/i2c-1
 *      /dev/i2c-2
 *
 *  Dependencies
 *  ------------
 *
 *      <linux/i2c-dev.h>
 *      <fcntl.h>
 *      <unistd.h>
 *      <sys/ioctl.h>
 *
 *  Thread Safety
 *  -------------
 *
 *  LinuxI2cBus instances are not internally synchronized.
 *  External synchronization is required if multiple threads share
 *  the same object.
 *
 ******************************************************************************/

namespace hal
{
    LinuxI2cBus::LinuxI2cBus(std::string  device): devicePath { std::move(device) }
    {
        if (Error::Success == open()) {
            throw std::runtime_error("Error in LinuxI2cBus::open()");
        }
    }

    LinuxI2cBus::~LinuxI2cBus()
    {
        if (fileDescriptor >= 0)
        {
            ::close(fileDescriptor);
        }
    }

    Error LinuxI2cBus::open()
    {
        fileDescriptor =::open(devicePath.c_str(),O_RDWR);
        if (fileDescriptor < 0){
            return Error::HardwareFailure;
        }

        return Error::Success;
    }

    Error LinuxI2cBus::selectDevice(const uint8_t address)
    {
        if (fileDescriptor < 0){
            return Error::HardwareFailure;
        }
        if (address == currentAddress){
            return Error::Success;
        }
        if (::ioctl(fileDescriptor, I2C_SLAVE, address) < 0){
            return Error::HardwareFailure;
        }

        currentAddress = address;
        return Error::Success;
    }


    Error LinuxI2cBus::write(const uint8_t address,
                             const uint8_t* data,
                             const uint32_t size)
    {
        const Error result = selectDevice(address);
        if (result != Error::Success){
            return result;
        }

        const ssize_t written = ::write(fileDescriptor, data, size);
        if (written != static_cast<ssize_t>(size))
        {
            return Error::HardwareFailure;
        }

        return Error::Success;
    }


    Error LinuxI2cBus::read(const uint8_t address,
                            uint8_t* data,
                            const uint32_t size)
    {
        const Error result = selectDevice(address);
        if (result != Error::Success){
            return result;
        }

        const ssize_t received = ::read(fileDescriptor, data, size);
        if (received != static_cast<ssize_t>(size)){
            return Error::HardwareFailure;
        }

        return Error::Success;
    }

    Error LinuxI2cBus::transfer(const uint8_t address,
                                const uint8_t* txData,
                                const uint32_t txSize,
                                uint8_t* rxData,
                                const uint32_t rxSize)
    {
        const Error result = write(address, txData,txSize);
        if (result != Error::Success) {
            return result;
        }
        return read(address, rxData, rxSize);
    }

    bool LinuxI2cBus::isOpen() const noexcept {
        return fileDescriptor >= 0;
    }

    const std::string& LinuxI2cBus::getDevicePath() const noexcept {
        return devicePath;
    }
}