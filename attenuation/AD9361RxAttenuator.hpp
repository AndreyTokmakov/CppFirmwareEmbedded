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

namespace attenuation
{
    class IAd9361RxGainControl
    {
    public:

        virtual ~IAd9361RxGainControl() = default;

        /**
         * Programs the receive attenuation of the AD9361 RX channel.
         *
         * The supplied attenuation value is assumed to be already validated by the
         * caller.
         *
         * @param attenuationDb Desired attenuation in dB.
         *
         * @return Operation status.
         */
        virtual Error SetRxAttenuation(double attenuationDb) = 0;

        /**
         * Returns the attenuation currently programmed into the RX channel.
         *
         * @return Current attenuation in dB.
         */
        [[nodiscard]]
        virtual double GetRxAttenuation() const = 0;
    };

    /**
     * Provides an IAttenuator adapter for the AD9361 receive signal path.
     *
     * Unlike dedicated RF attenuator ICs such as the PE4312, attenuation in the
     * AD9361 is implemented as part of the receiver gain control architecture.
     * This class exposes that functionality through the generic IAttenuator
     * interface, allowing application code to configure different RF devices
     * using the same API.
     *
     * Configuration changes are cached locally and applied to the hardware only
     * when Apply() is invoked.
     */
    class AD9361RxAttenuator : public AttenuatorBase
    {
    public:

        /**
         * Creates an attenuator adapter for the specified AD9361 RX channel.
         *
         * @param channel AD9361 receive channel implementation.
         */
        explicit AD9361RxAttenuator(IAd9361RxGainControl& channel);

        /**
         * Returns a human-readable device name.
         */
        [[nodiscard]]
        std::string GetName() const override;

        /**
         * Indicates whether attenuation can be modified.
         *
         * The AD9361 supports programmable receive attenuation and always
         * returns true.
         */
        [[nodiscard]]
        bool IsProgrammable() const override;

        /**
         * Requests a new attenuation value.
         *
         * The requested value is validated, quantized to the supported hardware
         * resolution and stored locally until Apply() is called.
         *
         * @param attenuationDb Desired attenuation in dB.
         *
         * @return Operation status.
         */
        Error SetAttenuation(double attenuationDb) override;

        /**
         * Returns the attenuation currently programmed into the hardware.
         */
        [[nodiscard]]
        double GetAttenuation() const override;

        /**
         * Returns the minimum supported attenuation.
         */
        [[nodiscard]]
        double GetMinimumAttenuation() const override;

        /**
         * Returns the maximum supported attenuation.
         */
        [[nodiscard]]
        double GetMaximumAttenuation() const override;

        /**
         * Returns the attenuation resolution.
         */
        [[nodiscard]]
        double GetStepSize() const override;

        /**
         * Applies the pending attenuation to the AD9361.
         *
         * If no configuration changes are pending, the function returns immediately.
         *
         * @return Operation status.
         */
        Error Apply() override;

        /**
         * Indicates whether a hardware update is currently in progress.
         */
        [[nodiscard]]
        bool IsBusy() const override;

        /**
         * Restores the default attenuation and applies it to the hardware.
         *
         * @return Operation status.
         */
        Error Reset() override;

    private:

        static constexpr double MinimumAttenuation = 0.0;
        static constexpr double MaximumAttenuation = 89.75;
        static constexpr double StepSize = 0.25;

    private:

        IAd9361RxGainControl& channel;

        /// Attenuation currently programmed into the hardware.
        double appliedAttenuation = 0.0;

        /// Attenuation waiting to be applied.
        double pendingAttenuation = 0.0;

        /// Indicates that the pending configuration has not yet been applied.
        bool configurationDirty = false;

        /// Indicates that a hardware update is currently in progress.
        bool updateInProgress = false;
    };
}

#endif //CPPFIRMWAREEMBEDDED_AD9361RXATTENUATOR_HPP
