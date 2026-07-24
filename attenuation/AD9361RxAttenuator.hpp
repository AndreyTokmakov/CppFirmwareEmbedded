/**============================================================================
Name        : AD9361RxAttenuator.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : AD9361RxAttenuator.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_AD9361RXATTENUATOR_HPP
#define CPPFIRMWAREEMBEDDED_AD9361RXATTENUATOR_HPP

#include <string>
#include "AttenuatorBase.hpp"

/**
==============================================================================
Design Notes
==============================================================================

The AD9361 should not be treated as a standalone RF attenuator.

Unlike dedicated digital step attenuators (e.g. PE4312, PE43711 or HMC624),
the AD9361 is a complete RF transceiver integrating multiple functional blocks,
including:

    - RX and TX signal chains
    - Low Noise Amplifiers (LNA)
    - Variable Gain Amplifiers (VGA)
    - Mixers
    - Analog Filters
    - ADC/DAC
    - Automatic Gain Control (AGC)
    - Manual Gain Control (MGC)
    - Numerous configuration registers

The receive attenuation (or hardware gain) is only one configurable parameter
within the RX signal chain rather than a dedicated hardware component.

For this reason AD9361RxAttenuator is implemented directly as an
IAttenuator/AttenuatorBase adapter instead of inheriting from any digital
attenuator implementation such as PE4312. Although both expose attenuation in
dB, the underlying hardware architecture and programming model are completely
different.

Another important design goal is to keep this library independent of any
specific AD9361 software stack.

The attenuator library should not depend directly on:

    - Analog Devices no-OS driver
    - libiio
    - Linux Industrial I/O (IIO)
    - vendor SDKs
    - platform-specific SPI implementations

Instead, communication with the actual transceiver is performed through a small
abstract interface (IAd9361). This keeps the attenuator implementation focused
only on attenuation-related functionality while delegating all hardware access
to the platform-specific driver.

This separation provides several advantages:

    - Hardware-independent attenuation logic
    - Easy unit testing using mock implementations
    - Portability across embedded platforms
    - Ability to support multiple AD9361 driver implementations without
      modifying the attenuator code
    - Clear separation between RF device logic and transport layer

In other words, AD9361RxAttenuator acts as an adapter between the generic
IAttenuator interface used by the application and the concrete AD9361 driver
used by the underlying platform.
**/

namespace rf
{
    class IAd9361
    {
    public:

        virtual ~IAd9361() = default;

        /**
         * @brief Sets RX attenuation (hardware gain) in Manual Gain Control mode.
         */
        virtual Error SetRxAttenuation(double attenuationDb) = 0;

        /**
         * @brief Returns current RX attenuation.
         */
        [[nodiscard]]
        virtual double GetRxAttenuation() const = 0;
    };

    /**
     * @brief AD9361 RX attenuator adapter.
     *
     * Provides a generic attenuator interface for the RX gain
     * control of the AD9361 transceiver.
     */
    class AD9361RxAttenuator : public AttenuatorBase
    {
    public:

        explicit AD9361RxAttenuator(IAd9361& device);

        /**
         * @brief Returns device name.
         */
        [[nodiscard]]
        std::string GetName() const override;

        /**
         * @brief AD9361 attenuation is programmable.
         */
        [[nodiscard]]
        bool IsProgrammable() const override;

        /**
         * @brief Sets RX attenuation.
         *
         * The value is stored locally until Apply() is called.
         */
        Error SetAttenuation(double attenuationDb) override;

        /**
         * @brief Returns currently applied attenuation.
         */
        [[nodiscard]]
        double GetAttenuation() const override;

        /**
         * @brief Returns minimum supported attenuation.
         */
        double GetMinimumAttenuation() const override;

        /**
         * @brief Returns maximum supported attenuation.
         */
        double GetMaximumAttenuation() const override;

        /**
         * @brief Returns attenuation resolution.
         */
        double GetStepSize() const override;

        /**
         * @brief Applies pending attenuation to the hardware.
         */
        Error Apply() override;

        /**
         * @brief Returns busy state.
         */
        bool IsBusy() const override;

        /**
         * @brief Restores default attenuation.
         */
        Error Reset() override;

    private:

        IAd9361& device;

        double currentAttenuation = 0.0;
        double pendingAttenuation = 0.0;

        bool dirty = false;
        bool busy = false;
    };
}

#endif //CPPFIRMWAREEMBEDDED_AD9361RXATTENUATOR_HPP
