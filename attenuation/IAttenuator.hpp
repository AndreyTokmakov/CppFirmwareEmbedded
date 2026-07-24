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

namespace rf
{   /**
     * @brief Generic interface for RF attenuators.
     * This interface abstracts fixed, programmable and integrated attenuators behind a common API.
     */
    struct IAttenuator
    {
        virtual ~IAttenuator() = default;

        /**
         * @brief Returns device name.
         */
        [[nodiscard]]
        virtual std::string GetName() const = 0;

        /**
         * @brief Returns true if attenuation can be changed.
         */
        [[nodiscard]]
        virtual bool IsProgrammable() const = 0;

        /**
         * @brief Sets attenuation.
         * @param attenuationDb Desired attenuation in dB.
         * @return Operation result.
         */
        virtual Error SetAttenuation(double attenuationDb) = 0;

        /**
         * @brief Returns current attenuation.
         */
        [[nodiscard]]
        virtual double GetAttenuation() const = 0;

        /**
         * @brief Returns minimum supported attenuation.
         */
        [[nodiscard]]
        virtual double GetMinimumAttenuation() const = 0;

        /**
         * @brief Returns maximum supported attenuation.
         */
        [[nodiscard]]
        virtual double GetMaximumAttenuation() const = 0;

        /**
         * @brief Returns attenuation resolution.
         * Fixed attenuators typically return 0.
         */
        [[nodiscard]]
        virtual double GetStepSize() const = 0;

        /**
         * @brief Checks whether the attenuation value is supported.
         * The hardware state must not be modified.
         */
        [[nodiscard]]
        virtual bool IsValidAttenuation(double attenuationDb) const = 0;

        /**
         * @brief Returns the closest attenuation value supported by the hardware.
         */
        [[nodiscard]]
        virtual double QuantizeAttenuation(double attenuationDb) const = 0;

        /**
         * @brief Enables RF signal path.
         */
        virtual Error Enable() = 0;

        /**
         * @brief Disables RF signal path.
         */
        virtual Error Disable() = 0;

        /**
         * @brief Returns current enable state.
         */
        [[nodiscard]]
        virtual bool IsEnabled() const = 0;

        /**
         * @brief Applies pending configuration to the hardware.
         * Devices that update immediately should simply return Error::None.
         */
        virtual Error Apply() = 0;

        /**
         * @brief Returns true if the device is currently updating its internal state.
         */
        [[nodiscard]]
        virtual bool IsBusy() const = 0;

        /**
         * @brief Restores the default hardware configuration.
         */
        virtual Error Reset() = 0;
    };

}

#endif //CPPFIRMWAREEMBEDDED_IATTENUATOR_HPP
