/**============================================================================
Name        : IDelay.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IDelay.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IDELAY_HPP
#define CPPFIRMWAREEMBEDDED_IDELAY_HPP

#include <cstdint>

namespace hal
{
    /**
     * Abstract delay interface.
     * Provides hardware-independent timing delays.
     * This interface is separated from ITimer because measuring time and waiting for time are different responsibilities.
     *
     * Possible implementations:
     *   - busy wait loop
     *   - MCU hardware timer
     *   - Linux nanosleep()
     *   - RTOS delay API
     *
     * RF devices often require precise delays:
     *   - reset pulse width
     *   - PLL settling time
     *   - SPI timing requirements
     *   - GPIO synchronization
     */
    struct IDelay
    {
        virtual ~IDelay() = default;

        /**
         * Blocks execution for the specified number of microseconds.
         * The implementation decides whether to use a busy wait or operating system sleep.
         * For RF initialization sequences a precise delay may be required.
         * @param microseconds Delay duration.
         */
        virtual void microseconds(uint32_t microseconds) = 0;

        /**
         * Blocks execution for the specified number of milliseconds.
         *
         * This function is intended for longer delays where microsecond
         * precision is not required.
         *
         * @param milliseconds Delay duration.
         */
        virtual void milliseconds(uint32_t milliseconds) = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_IDELAY_HPP
