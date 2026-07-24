/**============================================================================
Name        : PE4312.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE4312.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_PE4312_HPP
#define CPPFIRMWAREEMBEDDED_PE4312_HPP

#include <cstdint>

#include "AttenuatorBase.hpp"

namespace rf
{   /**
     * Represents the pSemi PE4312 digital step attenuator.
     *
     * The PE4312 is a digitally controlled RF step attenuator designed for
     * applications requiring accurate and repeatable signal level control.
     * The device is programmed through a serial interface and provides
     * programmable attenuation over a wide dynamic range.
     *
     * Device characteristics:
     *   - Attenuation range: 0.0 dB to 31.5 dB
     *   - Resolution: 0.5 dB
     *   - 6-bit attenuation control word
     *   - Low insertion loss
     *   - High attenuation accuracy
     *   - Suitable for frequencies from DC up to several GHz
     *
     * Typical applications:
     *   - Software Defined Radio (SDR)
     *   - RF front-end gain control
     *   - Automatic Gain Control (AGC)
     *   - Test and measurement equipment
     *   - Radar systems
     *   - Wireless communication systems
     *
     * This class separates the device logic from the hardware transport layer.
     * Attenuation values are validated and converted into the corresponding
     * control word, while the actual SPI transaction is delegated to
     * WriteAttenuationCode(), allowing the driver to be reused on different
     * platforms.
     *
     * The requested attenuation is cached locally and is not written to the
     * hardware until Apply() is called. This allows multiple configuration
     * changes to be committed as a single hardware transaction.
     */
    class PE4312 : public AttenuatorBase
    {
    public:

        /**
         * Creates a new PE4312 driver instance.
         */
        PE4312();

        /**
         * Returns a human-readable device name.
         */
        [[nodiscard]]
        std::string GetName() const override;

        /**
         * Indicates whether the attenuation level can be modified.
         *
         * The PE4312 is a fully programmable digital attenuator and always
         * returns true.
         */
        [[nodiscard]]
        bool IsProgrammable() const override;

        /**
         * Requests a new attenuation value.
         *
         * The requested value is validated against the supported operating
         * range and rounded to the nearest value representable by the device.
         *
         * The hardware is not accessed immediately. The new attenuation is
         * stored internally and becomes active only after Apply() is called.
         *
         * @param attenuationDb Desired attenuation in dB.
         *
         * @return Operation status.
         */
        Error SetAttenuation(double attenuationDb) override;

        /**
         * Returns the attenuation currently programmed into the device.
         *
         * The returned value reflects the active hardware configuration
         * rather than any pending changes.
         */
        [[nodiscard]]
        double GetAttenuation() const override;

        /**
         * Returns the minimum attenuation supported by the device.
         */
        [[nodiscard]]
        double GetMinimumAttenuation() const override;

        /**
         * Returns the maximum attenuation supported by the device.
         */
        [[nodiscard]]
        double GetMaximumAttenuation() const override;

        /**
         * Returns the attenuation resolution.
         *
         * The PE4312 supports attenuation increments of 0.5 dB.
         */
        [[nodiscard]]
        double GetStepSize() const override;

        /**
         * Applies the pending attenuation to the hardware.
         *
         * If no configuration changes are pending, the function returns
         * immediately without performing any hardware access.
         *
         * @return Operation status.
         */
        Error Apply() override;

        /**
         * Indicates whether a hardware update is currently in progress.
         *
         * This function is primarily intended for asynchronous or
         * platform-specific implementations.
         */
        [[nodiscard]]
        bool IsBusy() const override;

        /**
         * Restores the default device configuration.
         *
         * The PE4312 powers up with 0 dB attenuation. Calling Reset()
         * restores this configuration and applies it to the hardware.
         *
         * @return Operation status.
         */
        Error Reset() override;

    protected:

        /**
         * Writes an encoded attenuation control word to the device.
         *
         * This function forms the hardware abstraction layer of the driver.
         * Platform-specific implementations are responsible for performing
         * the actual SPI transaction.
         *
         * @param controlWord Encoded attenuation control word.
         *
         * @return Operation status.
         */
        virtual Error WriteAttenuationCode(uint8_t controlWord) = 0;

    private:

        /**
         * Builds a PE4312 attenuation control word from the specified
         * attenuation value.
         *
         * The returned value can be transmitted directly to the device
         * through the serial interface.
         *
         * @param attenuationDb Attenuation in dB.
         *
         * @return Encoded attenuation control word.
         */
        [[nodiscard]]
        static uint8_t BuildControlWord(double attenuationDb);

    private:

        /// Attenuation currently programmed into the device.
        double appliedAttenuation = 0.0;

        /// Attenuation waiting to be written to the hardware.
        double pendingAttenuation = 0.0;

        /// Indicates that the pending configuration has not yet been applied.
        bool configurationDirty = false;

        /// Indicates that a hardware update is currently in progress.
        bool updateInProgress = false;

        static constexpr double MinimumAttenuation = 0.0;
        static constexpr double MaximumAttenuation = 31.5;
        static constexpr double StepSize = 0.5;
    };
}

#endif //CPPFIRMWAREEMBEDDED_PE4312_HPP
