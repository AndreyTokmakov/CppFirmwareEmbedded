/**============================================================================
Name        : FixedAttenuator.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : FixedAttenuator.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_FIXEDATTENUATOR_HPP
#define CPPFIRMWAREEMBEDDED_FIXEDATTENUATOR_HPP

#include "AttenuatorBase.hpp"

namespace rf
{   /**
     * @brief Represents a fixed RF attenuator.
     *
     * The attenuation value is defined at construction time and cannot
     * be modified during runtime.
     */
    class FixedAttenuator : public AttenuatorBase
    {
    public:

        /**
         * @brief Creates a fixed attenuator.
         *
         * @param attenuationDb Fixed attenuation in dB.
         */
        explicit FixedAttenuator(double attenuationDb);

        /**
         * @brief Returns device name.
         */
        std::string GetName() const override;

        /**
         * @brief Returns false since this device cannot be reconfigured.
         */
        bool IsProgrammable() const override;

        /**
         * @brief Fixed attenuators do not support changing attenuation.
         *
         * @return Error::NotSupported.
         */
        Error SetAttenuation(double attenuationDb) override;

        /**
         * @brief Returns the fixed attenuation value.
         */
        double GetAttenuation() const override;

        /**
         * @brief Returns the minimum supported attenuation.
         */
        double GetMinimumAttenuation() const override;

        /**
         * @brief Returns the maximum supported attenuation.
         */
        double GetMaximumAttenuation() const override;

        /**
         * @brief Returns attenuation resolution.
         *
         * Always returns 0 for fixed attenuators.
         */
        double GetStepSize() const override;

        /**
         * @brief Restores the default device state.
         *
         * No hardware operation is required.
         */
        Error Reset() override;

    private:

        double attenuation;
    };
}

#endif //CPPFIRMWAREEMBEDDED_FIXEDATTENUATOR_HPP
