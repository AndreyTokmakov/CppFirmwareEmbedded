/**============================================================================
Name        : ITimer.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : ITimer.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_ITIMER_HPP
#define CPPFIRMWAREEMBEDDED_ITIMER_HPP

#include <cstdint>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * Abstract timer interface.
     *
     * Provides access to a monotonic hardware timer.
     *
     * This interface is intended for:
     *
     *   - timeout handling
     *   - measuring operation duration
     *   - performance profiling
     *   - RF calibration sequences
     *   - communication protocol timing
     *
     * The timer must not depend on system time or wall clock.
     *
     * The returned value should only move forward.
     *
     * Possible implementations:
     *   - CPU cycle counter
     *   - MCU hardware timer
     *   - Linux clock_gettime()
     *   - FPGA timestamp counter
     */
    struct ITimer
    {
        virtual ~ITimer() = default;

        /**
         * Returns current timer value.
         * The value represents elapsed time since timer initialization.
         * The resolution depends on the implementation.
         *
         * Example:
         *   uint64_t start = timer.GetTicks();
         *   operation();
         *   uint64_t elapsed = timer.GetTicks() - start;
         *
         * @return Current timer tick value.
         */
        [[nodiscard]]
        virtual uint64_t getTicks() const = 0;

        /**
         * Returns timer frequency.
         * This value is used to convert ticks into time units.
         *
         * Example:
         *   frequency = 1000000
         *   1000 ticks = 1 ms
         *
         * @return Timer frequency in Hz.
         */
        [[nodiscard]]
        virtual uint64_t getFrequency() const = 0;

        /**
         * Returns elapsed time in microseconds.
         *
         * This method provides a convenient conversion from hardware
         * timer ticks into a human-readable time unit.
         *
         * @return Elapsed microseconds.
         */
        [[nodiscard]]
        virtual uint64_t getMicroseconds() const = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_ITIMER_HPP
