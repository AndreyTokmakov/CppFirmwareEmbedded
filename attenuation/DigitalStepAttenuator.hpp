/**============================================================================
Name        : DigitalStepAttenuator.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : DigitalStepAttenuator.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_DIGITALSTEPATTENUATOR_HPP
#define CPPFIRMWAREEMBEDDED_DIGITALSTEPATTENUATOR_HPP

#include <cstdint>

#include "AttenuatorBase.hpp"

namespace attenuation
{
    /**
     * Represents a programmable digital step attenuator.
     *
     * This class implements the common functionality shared by most digitally
     * controlled RF attenuators. It provides attenuation validation,
     * quantization, deferred configuration updates and state management,
     * while delegating device-specific operations to derived classes.
     *
     * Typical supported devices include:
     *   - pSemi PE4312
     *   - pSemi PE43711
     *   - Analog Devices HMC540
     *   - Analog Devices HMC624
     *   - Analog Devices ADRF5730
     *
     * Configuration changes are cached locally and become effective only
     * after Apply() is called. This allows multiple configuration changes
     * to be committed as a single hardware transaction.
     *
     * The class is independent of the underlying hardware interface.
     * Derived classes may communicate with the device using SPI, GPIO,
     * parallel interfaces or any other transport mechanism.
     */
    class DigitalStepAttenuator : public AttenuatorBase
    {
    public:

        /**
         * Creates a programmable digital step attenuator.
         *
         * @param minimumAttenuation Minimum supported attenuation in dB.
         * @param maximumAttenuation Maximum supported attenuation in dB.
         * @param stepSize Attenuation resolution in dB.
         */
        DigitalStepAttenuator(double minimumAttenuation,
                              double maximumAttenuation,
                              double stepSize);

        /**
         * Indicates whether the attenuation level can be modified.
         *
         * Digital step attenuators are fully programmable and therefore
         * always return true.
         *
         * @return true.
         */
        [[nodiscard]]
        bool IsProgrammable() const override;

        /**
         * Requests a new attenuation value.
         *
         * The requested value is validated against the supported operating
         * range, rounded to the nearest value representable by the hardware
         * and stored internally.
         *
         * The hardware is not accessed until Apply() is called.
         *
         * @param attenuationDb Desired attenuation in dB.
         *
         * @return Operation status.
         */
        Error SetAttenuation(double attenuationDb) override;

        /**
         * Returns the attenuation currently programmed into the hardware.
         *
         * @return Current attenuation in dB.
         */
        [[nodiscard]]
        double GetAttenuation() const override;

        /**
         * Returns the minimum attenuation supported by the device.
         *
         * @return Minimum attenuation in dB.
         */
        [[nodiscard]]
        double GetMinimumAttenuation() const override;

        /**
         * Returns the maximum attenuation supported by the device.
         *
         * @return Maximum attenuation in dB.
         */
        [[nodiscard]]
        double GetMaximumAttenuation() const override;

        /**
         * Returns the attenuation resolution.
         *
         * @return Attenuation step size in dB.
         */
        [[nodiscard]]
        double GetStepSize() const override;

        /**
         * Applies the pending attenuation to the hardware.
         *
         * The attenuation value is first converted into a device-specific
         * attenuation code, then packed into the final control word and
         * finally programmed into the device.
         *
         * If no configuration changes are pending, the function returns
         * immediately without accessing the hardware.
         *
         * @return Operation status.
         */
        Error Apply() override;

        /**
         * Indicates whether a hardware update is currently in progress.
         *
         * @return true if the device is currently being programmed.
         */
        [[nodiscard]]
        bool IsBusy() const override;

        /**
         * Restores the default attenuation and applies it to the hardware.
         *
         * The default attenuation corresponds to the minimum attenuation
         * supported by the device.
         *
         * @return Operation status.
         */
        Error Reset() override;

    protected:

        /**
         * Encodes an attenuation value into a device-specific attenuation code.
         *
         * The returned value represents only the attenuation code and does
         * not necessarily correspond to the complete hardware control word.
         *
         * @param attenuationDb Attenuation in dB.
         *
         * @return Encoded attenuation code.
         */
        [[nodiscard]]
        virtual uint16_t EncodeAttenuation(double attenuationDb) const = 0;

        /**
         * Builds the complete hardware control word from the encoded
         * attenuation code.
         *
         * The default implementation returns the attenuation code unchanged.
         * Devices requiring additional control bits, register addresses,
         * parity bits or command fields may override this method.
         *
         * @param attenuationCode Encoded attenuation code.
         *
         * @return Device-specific control word.
         */
        [[nodiscard]]
        virtual uint16_t BuildControlWord(uint16_t attenuationCode) const;

        /**
         * Programs the specified control word into the hardware.
         *
         * Derived classes are responsible for performing the actual hardware
         * transaction required by the target platform.
         *
         * @param controlWord Device-specific control word.
         *
         * @return Operation status.
         */
        virtual Error ProgramControlWord(uint16_t controlWord) = 0;

    private:

        const double minimumAttenuation;
        const double maximumAttenuation;
        const double stepSize;

    protected:

        /// Attenuation currently programmed into the hardware.
        double appliedAttenuation = 0.0;

        /// Attenuation waiting to be programmed.
        double pendingAttenuation = 0.0;

        /// Indicates that the pending configuration has not yet been applied.
        bool configurationDirty = false;

        /// Indicates that a hardware update is currently in progress.
        bool updateInProgress = false;
    };
}

#endif //CPPFIRMWAREEMBEDDED_DIGITALSTEPATTENUATOR_HPP
