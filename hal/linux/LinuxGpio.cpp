/**============================================================================
Name        : LinuxGpio.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxGpio.cpp
============================================================================**/

#include "LinuxGpio.hpp"
#include <gpiod.h>

/******************************************************************************
 *
 *  LinuxGpio.cpp
 *
 *  Linux implementation of the HAL GPIO interface.
 *
 *  Overview
 *  --------
 *
 *  This module provides a Linux-specific implementation of the IGpio
 *  abstraction using the libgpiod library.
 *
 *  The purpose of this implementation is to expose a platform-independent
 *  GPIO API to higher software layers while hiding all Linux-specific
 *  details such as GPIO character devices, file descriptors and libgpiod
 *  data structures.
 *
 *  Higher-level libraries (RF drivers, firmware components, protocol
 *  implementations, etc.) should communicate only through the IGpio
 *  interface and must never directly depend on Linux GPIO APIs.
 *
 *  Typical Use Cases
 *  -----------------
 *
 *  This implementation is intended for controlling digital signals such as:
 *      • Chip Select (CS)
 *      • Latch Enable (LE)
 *      • Reset
 *      • Enable / Disable
 *      • Power Down
 *      • Synchronization signals
 *      • Interrupt inputs
 *      • Status pins
 *
 *  Typical RF devices using GPIO:
 *      • PE4312
 *      • PE43711
 *      • AD9361
 *      • ADF4351
 *      • ADF5355
 *      • RF switches
 *      • Clock generators
 *
 *  Linux GPIO API
 *  --------------
 *
 *  This implementation uses the Linux GPIO Character Device interface
 *  through the libgpiod userspace library.
 *
 *  The legacy sysfs GPIO interface is intentionally not supported because
 *  it has been deprecated by the Linux kernel.
 *
 *  The implementation expects GPIO devices to be available as:
 *
 *      /dev/gpiochip0
 *      /dev/gpiochip1
 *      ...
 *
 *  Thread Safety
 *  -------------
 *
 *  Instances of LinuxGpio are not internally synchronized.
 *
 *  If multiple threads access the same GPIO object simultaneously,
 *  synchronization must be provided by the caller.
 *
 *  Resource Management
 *  -------------------
 *
 *  The GPIO line is acquired during object construction and released
 *  automatically when the object is destroyed.
 *
 *  This class follows the RAII principle to ensure proper cleanup even
 *  when exceptions or early returns occur.
 *
 *  Error Handling
 *  --------------
 *
 *  Linux-specific errors returned by libgpiod are translated into the
 *  platform-independent Error enumeration defined by the HAL library.
 *
 *  The caller should never rely on Linux errno values.
 *
 *  Dependencies
 *  ------------
 *
 *  Required Linux packages:
 *
 *      libgpiod
 *      libgpiod-dev
 *
 *  Required headers:
 *
 *      <gpiod.h>
 *
 *  Required kernel support:
 *
 *      GPIO Character Device API
 *
 *  Platform
 *  --------
 *
 *  Operating System:
 *
 *      Linux
 *
 *  This module is not intended for:
 *
 *      • STM32
 *      • Bare-metal firmware
 *      • RTOS environments
 *      • Zephyr
 *
 *  Those platforms should provide their own IGpio implementation.
 *
 ******************************************************************************/

namespace hal
{
    LinuxGpio::LinuxGpio(const std::string& chipName,uint16_t lineNumber)
    {
        chip = gpiod_chip_open_by_name(chipName.c_str());
        if (chip == nullptr){
            return;
        }

        line = gpiod_chip_get_line(chip,lineNumber);
    }

    LinuxGpio::~LinuxGpio()
    {
        if (line != nullptr) {
            gpiod_line_release(line);
        }
        if (chip != nullptr) {
            gpiod_chip_close(chip);
        }
    }

    Error LinuxGpio::configureOutput(const bool initialState)
    {
        if (line == nullptr) {
            return Error::HardwareFailure;
        }

        const int result = gpiod_line_request_output(line, "libhal", initialState ? 1 : 0);
        if (result < 0) {
            return Error::HardwareFailure;
        }

        state = initialState;
        return Error::Success;
    }

    Error LinuxGpio::configureInput()
    {
        if (line == nullptr) {
            return Error::HardwareFailure;
        }

        const int result = gpiod_line_request_input(line, "libhal");
        if (result < 0){
            return Error::HardwareFailure;
        }

        return Error::Success;
    }

    Error LinuxGpio::set(const bool high)
    {
        if (line == nullptr) {
            return Error::HardwareFailure;
        }

        const int result = gpiod_line_set_value(line, high ? 1 : 0);
        if (result < 0) {
            return Error::HardwareFailure;
        }

        state = high;
        return Error::Success;
    }

    bool LinuxGpio::get() const
    {
        if (line == nullptr){
            return false;
        }

        const int value = gpiod_line_get_value(line);
        if (value < 0){
            return false;
        }

        return value != 0;
    }

    bool LinuxGpio::isOpen() const noexcept {
        return line != nullptr;
    }

} // namespace hal