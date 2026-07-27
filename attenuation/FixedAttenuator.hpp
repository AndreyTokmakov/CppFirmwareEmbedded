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

/**
 * Represents a fixed RF attenuator.
 *
 * Overview
 * --------
 *
 * A fixed attenuator provides a constant attenuation determined entirely by
 * its internal passive resistor network. Unlike programmable attenuators,
 * the attenuation value cannot be changed after manufacturing and no digital
 * control interface is present.
 *
 * Fixed attenuators are widely used in RF systems to reduce signal levels,
 * improve impedance matching, protect sensitive receiver stages and extend
 * the operating range of measurement equipment.
 *
 * Since the attenuation is a physical property of the device, every instance
 * of this class represents a single fixed attenuation value supplied during
 * construction.
 *
 * Typical Applications
 * --------------------
 *
 * Fixed attenuators are commonly used for:
 *
 *     • Receiver input protection
 *     • Signal level adjustment
 *     • Instrument calibration
 *     • Test and measurement equipment
 *     • RF laboratory setups
 *     • Production test fixtures
 *
 * Hardware Model
 * --------------
 *
 * A fixed attenuator contains no programmable registers, control interface or
 * internal state. The attenuation is entirely defined by the hardware.
 *
 * Consequently:
 *
 *     • attenuation cannot be modified
 *     • no communication interface is required
 *     • no hardware update is performed
 *
 * Thread Safety
 * -------------
 *
 * This class is immutable after construction and therefore inherently
 * thread-safe.
 */

namespace attenuation
{
    class FixedAttenuator : public AttenuatorBase
    {
    public:

        /**
         * Creates a fixed attenuator.
         *
         * @param attenuationDb Fixed attenuation in decibels.
         */
        explicit FixedAttenuator(double attenuationDb);

        /**
         * Returns a human-readable device name.
         *
         * @return Device name.
         */
        [[nodiscard]]
        std::string getName() const override;

        /**
         * Indicates whether the attenuation can be modified.
         *
         * Fixed attenuators always return false because the attenuation is a
         * physical property of the device.
         *
         * @return Always false.
         */
        [[nodiscard]]
        bool isProgrammable() const override;

        /**
         * Attempts to change the attenuation.
         *
         * Since the attenuation is fixed by the hardware, the requested value is
         * ignored and Error::NotSupported is returned.
         *
         * @param attenuationDb Requested attenuation in decibels.
         *
         * @return Error::NotSupported.
         */
        Error setAttenuation(double attenuationDb) override;

        /**
         * Returns the attenuation provided by the hardware.
         *
         * @return Fixed attenuation in decibels.
         */
        [[nodiscard]]
        double getAttenuation() const override;

        /**
         * Returns the minimum supported attenuation.
         *
         * Since only one attenuation value exists, this function returns the same
         * value as getMaximumAttenuation().
         *
         * @return Minimum attenuation in decibels.
         */
        [[nodiscard]]
        double getMinimumAttenuation() const override;

        /**
         * Returns the maximum supported attenuation.
         *
         * Since only one attenuation value exists, this function returns the same
         * value as getMinimumAttenuation().
         *
         * @return Maximum attenuation in decibels.
         */
        [[nodiscard]]
        double getMaximumAttenuation() const override;

        /**
         * Returns the attenuation resolution.
         *
         * Fixed attenuators are not programmable and therefore have no attenuation
         * step size.
         *
         * @return Always 0.0.
         */
        [[nodiscard]]
        double getStepSize() const override;

    private:

        double attenuation;
    };
}

#endif //CPPFIRMWAREEMBEDDED_FIXEDATTENUATOR_HPP
