/**============================================================================
Name        : IAttenuator.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IAttenuator.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IATTENUATOR_HPP
#define CPPFIRMWAREEMBEDDED_IATTENUATOR_HPP

#include <string>

#include "Error.hpp"

namespace attenuation
{
    using common::Error;

    /**
     * @brief Generic interface for RF attenuators.
     * This interface abstracts fixed, programmable and integrated attenuators behind a common API.
     */
     struct IAttenuator
    {
        virtual ~IAttenuator() = default;

        /**
         * Returns a human-readable device name.
         *
         * The returned string is intended for logging, diagnostics, user interfaces
         * and debugging. It should uniquely identify the attenuator model whenever possible.
         */
        [[nodiscard]]
        virtual std::string GetName() const = 0;

        /**
         * Indicates whether the attenuation level can be modified during runtime.
         *
         * Returns false for fixed attenuators whose attenuation is permanently defined
         * by the hardware.
         */
        [[nodiscard]]
        virtual bool IsProgrammable() const = 0;

        /**
         * Requests a new attenuation value expressed in decibels.
         *
         * The implementation may either apply the value immediately or cache it until
         * Apply() is called, depending on the device.
         *
         * The requested value must be validated against the supported attenuation range
         * and hardware resolution.
         *
         * @param attenuationDb Desired attenuation in dB.
         *
         * @return Operation status.
         */
        virtual Error SetAttenuation(double attenuationDb) = 0;

        /**
         * Returns the attenuation currently configured by the device.
         *
         * The returned value represents the active attenuation rather than the last
         * value requested by SetAttenuation().
         */
        [[nodiscard]]
        virtual double GetAttenuation() const = 0;

        /**
         * Returns the minimum attenuation supported by the hardware.
         *
         * Together with GetMaximumAttenuation(), this defines the valid operating
         * range of the attenuator.
         */
        [[nodiscard]]
        virtual double GetMinimumAttenuation() const = 0;

        /**
         * Returns the maximum attenuation supported by the hardware.
         *
         * Any value above this limit shall be rejected by SetAttenuation().
         */
        [[nodiscard]]
        virtual double GetMaximumAttenuation() const = 0;

        /**
         * Returns the smallest attenuation increment supported by the hardware.
         *
         * Programmable attenuators usually expose a fixed step size, while fixed
         * attenuators typically return 0.0.
         */
        [[nodiscard]]
        virtual double GetStepSize() const = 0;

        /**
         * Checks whether a given attenuation value is supported.
         *
         * This function performs validation only and must not modify the hardware
         * state or any internal configuration.
         *
         * @param attenuationDb Attenuation value to validate.
         *
         * @return True if the value can be represented by the device.
         */
        [[nodiscard]]
        virtual bool IsValidAttenuation(double attenuationDb) const = 0;

        /**
         * Converts an arbitrary attenuation value into the closest value representable
         * by the hardware.
         *
         * For example, a device supporting 0.5 dB resolution may quantize 12.37 dB
         * to 12.5 dB.
         *
         * @param attenuationDb Requested attenuation.
         *
         * @return Nearest supported attenuation value.
         */
        [[nodiscard]]
        virtual double QuantizeAttenuation(double attenuationDb) const = 0;

        /**
         * Enables the RF signal path.
         *
         * Some attenuators support enabling or disabling the RF path, while others
         * may always remain enabled. Devices that do not implement this functionality
         * should return Error::NotSupported.
         */
        virtual Error Enable() = 0;

        /**
         * Disables the RF signal path.
         *
         * When supported, the attenuator should isolate or mute the RF path according
         * to the hardware capabilities.
         *
         * Devices that cannot disable the RF path should return Error::NotSupported.
         */
        virtual Error Disable() = 0;

        /**
         * Returns whether the RF signal path is currently enabled.
         *
         * For devices that do not support enabling or disabling, this function may
         * always return true.
         */
        [[nodiscard]]
        virtual bool IsEnabled() const = 0;

        /**
         * Applies any pending configuration changes to the hardware.
         *
         * Some implementations defer hardware access until Apply() is called, allowing
         * multiple parameters to be updated as a single transaction.
         *
         * Devices that update immediately may simply return Error::Success.
         */
        virtual Error Apply() = 0;

        /**
         * Indicates whether the device is currently performing an internal update.
         *
         * This can be useful for asynchronous implementations or hardware requiring
         * a finite settling time.
         */
        [[nodiscard]]
        virtual bool IsBusy() const = 0;

        /**
         * Restores the device to its default operating state.
         *
         * The exact meaning of "default" depends on the hardware implementation, but
         * typically includes restoring the default attenuation and clearing any
         * pending configuration.
         *
         * @return Operation status.
         */
        virtual Error Reset() = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_IATTENUATOR_HPP
