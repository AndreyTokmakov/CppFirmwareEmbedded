#pragma once

#include <chrono>
#include <cstdint>

/**
 * @file IPll.hpp
 *
 * @brief Common interface for Phase-Locked Loop (PLL) devices.
 *
 * A Phase-Locked Loop (PLL) is an electronic control system that generates
 * a stable output clock or local oscillator (LO) synchronized to a reference
 * clock. PLLs are widely used in embedded systems, RF front-ends, FPGA designs,
 * SoCs and high-speed digital interfaces to generate clocks with the required
 * frequency and stability.
 *
 * Typical applications include:
 *  - RF frequency synthesizers
 *      - Analog Devices ADF4351
 *      - Analog Devices ADF4371
 *      - Texas Instruments LMX2594
 *      - HMC833
 *
 *  - Internal MCU/SoC clock generators
 *      - STM32 RCC PLL
 *      - NXP i.MX PLL
 *      - Zynq PS PLL
 *
 *  - SDR and RF transceivers
 *      - AD9361
 *      - ADRV9002
 *      - LMS7002M
 *
 * This interface intentionally exposes only the common functionality shared by
 * almost every PLL implementation:
 *
 *  - Configure the output frequency.
 *  - Query the configured frequency.
 *  - Check whether the PLL has acquired lock.
 *  - Wait until lock is achieved.
 *  - Reset the device.
 *  - Obtain the current device status.
 *
 * Device-specific functionality such as:
 *
 *  - Fractional-N configuration
 *  - Integer-N mode
 *  - Charge Pump current
 *  - Loop Filter parameters
 *  - MUXOUT configuration
 *  - Output power
 *  - Output divider selection
 *  - Reference doubler/divider
 *  - Phase adjustment
 *
 * should be implemented by concrete driver classes rather than exposed by the
 * common interface.
 *
 * The purpose of this interface is to provide a lightweight abstraction that
 * allows firmware components to work with different PLL implementations without
 * depending on a specific hardware device.
 */


namespace rf
{
    using Frequency = uint64_t;

    enum class PllState
    {
        Uninitialized,
        Unlocked,
        Locked,
        Error
    };

    struct PllStatus
    {
        PllState state { PllState::Uninitialized };
        Frequency frequency { 0 };
    };

    class IPll
    {
    public:

        virtual ~IPll() = default;

        /**
         * @brief Reset the PLL.
         *
         * Restores the device to its default state.
         *
         * The exact behavior is implementation-defined and depends on the target
         * hardware. A reset may:
         *   - restore factory register values;
         *   - disable PLL outputs;
         *   - clear pending error conditions;
         *   - force the PLL into an unlocked state;
         *   - require the output frequency to be configured again.
         *
         * After calling this function the caller should not assume that the PLL
         * remains locked or continues generating the previously configured output
         * frequency.
         */
        virtual void reset() = 0;

        /**
          * @brief Configure the output frequency.
          *
          * Programs the PLL to generate the specified output frequency.
          *
          * Depending on the hardware implementation this operation may:
          *   - update divider values;
          *   - reconfigure the VCO;
          *   - switch output dividers;
          *   - restart the locking procedure.
          *
          * A successful return value only indicates that the configuration was
          * accepted by the driver. It does not guarantee that the PLL has already
          * acquired lock. Applications requiring a stable output should call
          * isLocked() or waitLock().
          *
          * @param frequency
          *     Desired output frequency in Hertz.
          *
          * @return
          *     true if the configuration was successfully accepted.
          *     false if the requested frequency is invalid or cannot be generated
          *     by the underlying hardware.
          */
        virtual bool setFrequency(Frequency frequency) = 0;

        /**
         * @brief Returns the configured output frequency.
         *
         * Returns the frequency currently programmed into the device.
         *
         * This value represents the requested output frequency and does not
         * necessarily indicate that the PLL is currently locked.
         *
         * @return
         *     Configured output frequency in Hertz.
         */
        virtual Frequency frequency() const = 0;

        /**
         * @brief Returns the current lock state.
         *
         * Indicates whether the PLL has successfully synchronized its output with
         * the reference clock.
         *
         * When this function returns true, the output frequency is considered
         * stable and ready for use.
         *
         * @return
         *     true if the PLL is locked.
         *     false otherwise.
         */
        virtual bool isLocked() const = 0;

        /**
         * @brief Wait until the PLL acquires lock.
         *
         * Blocks until one of the following conditions occurs:
         *   - the PLL reports a locked state;
         *   - the specified timeout expires.
         *
         * Implementations may poll hardware status registers, wait for an interrupt,
         * or use any other synchronization mechanism supported by the platform.
         *
         * @param timeout
         *     Maximum amount of time to wait.
         *
         * @return
         *     true if the PLL became locked before the timeout expired.
         *     false if the timeout expired or the lock operation failed.
         */
        virtual bool waitLock(std::chrono::milliseconds timeout) = 0;

        /**
         * @brief Returns the current runtime status.
         *
         * Provides a snapshot of the PLL state.
         *
         * The returned information is intended for diagnostics, monitoring and
         * debugging. Additional fields may be added to PllStatus in future
         * revisions without affecting the interface itself.
         *
         * @return
         *     Current PLL status.
         */
        virtual PllStatus status() const = 0;
    };
} // namespace rf