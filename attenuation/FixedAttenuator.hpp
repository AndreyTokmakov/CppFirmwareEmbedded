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

namespace attenuation
{   /**
     * Represents a fixed RF attenuator.
     *
     * A fixed attenuator provides a constant attenuation level that is determined
     * by the hardware and cannot be changed during runtime.
     *
     * Such attenuators are commonly used to:
     *  - protect sensitive RF inputs from excessive signal levels;
     *  - improve impedance matching between RF components;
     *  - reduce signal power by a known amount;
     *  - simulate cable or path losses during testing;
     *  - calibrate RF measurement equipment.
     *
     * Unlike programmable attenuators, this device always operates with the same
     * attenuation value that is specified during construction.
     *
     * Typical examples include Mini-Circuits VAT series, Pasternack PE70xx series
     * and other passive RF attenuators.
     */
    class FixedAttenuator : public AttenuatorBase
    {
    public:

        /**
         * Creates a fixed attenuator instance.
         *
         * @param attenuationDb Fixed attenuation in dB.
         */
        explicit FixedAttenuator(double attenuationDb);

        /**
         * Returns a human-readable device name.
         */
        [[nodiscard]]
        std::string GetName() const override;

        /**
         * Indicates whether the attenuation level can be modified.
         *
         * Fixed attenuators always return false because their attenuation
         * is determined entirely by the hardware.
         */
        [[nodiscard]]
        bool IsProgrammable() const override;

        /**
         * Attempts to change the attenuation level.
         *
         * Since the attenuation of this device is fixed by design, the
         * requested value is ignored and Error::NotSupported is returned.
         *
         * @param attenuationDb Requested attenuation in dB.
         *
         * @return Error::NotSupported.
         */
        Error SetAttenuation(double attenuationDb) override;

        /**
         * Returns the fixed attenuation value configured during construction.
         */
        [[nodiscard]]
        double GetAttenuation() const override;

        /**
         * Returns the minimum supported attenuation.
         *
         * Since the attenuation is fixed, this value is identical to GetMaximumAttenuation().
         */
        [[nodiscard]]
        double GetMinimumAttenuation() const override;

        /**
         * Returns the maximum supported attenuation.
         *
         * Since the attenuation is fixed, this value is identical to GetMinimumAttenuation().
         */
        [[nodiscard]]
        double GetMaximumAttenuation() const override;

        /**
         * Returns the attenuation resolution.
         *
         * Fixed attenuators have no programmable resolution and therefore always return 0.0.
         */
        [[nodiscard]]
        double GetStepSize() const override;

        /**
         * Restores the default operating state.
         *
         * No hardware configuration is required because the attenuation is permanently defined by the device.
         */
        Error Reset() override;

    private:

        double attenuation;
    };
}

#endif //CPPFIRMWAREEMBEDDED_FIXEDATTENUATOR_HPP
