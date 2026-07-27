/**============================================================================
Name        : AttenuatorBase.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : AttenuatorBase.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_ATTENUATORBASE_HPP
#define CPPFIRMWAREEMBEDDED_ATTENUATORBASE_HPP

#include <algorithm>
#include <cmath>
#include "IAttenuator.hpp"

/**
  * Base implementation of the IAttenuator interface.
  *
  * Overview
  * --------
  *
  * This class provides functionality shared by most RF attenuator implementations
  * while remaining completely independent of any particular hardware.
  *
  * The primary purpose of this class is to eliminate duplicated code
  * between different attenuator drivers.
  *
  * Implemented functionality
  * -------------------------
  *
  * The base class provides generic implementations for:
  *
  *     • attenuation range validation
  *     • attenuation quantization
  *
  * Both operations rely only on the minimum attenuation, maximum
  * attenuation and attenuation step reported by the derived class.
  *
  * Hardware-specific operations such as changing attenuation, reading registers or
  * communicating over SPI are intentionally left to the concrete implementations.
  *
  * Quantization
  * ------------
  *
  * Programmable attenuators usually support only discrete attenuation values.
  *
  * Examples:
  *     0.0
  *     0.5
  *     1.0
  *     1.5
  *     ...
  * or
  *     0.00
  *     0.25
  *     0.50
  *     0.75
  *     ...
  *
  * The quantizeAttenuation() function clamps the requested attenuation
  * into the supported range and rounds it to the nearest supported step.
  *
  *
  * Fixed Attenuators
  * -----------------
  * Fixed attenuators report a step size of 0.0.
  *
  * In this case the only valid attenuation is the fixed attenuation value itself,
  * therefore quantizeAttenuation() simply returns the minimum supported attenuation.
  *
  * Thread Safety
  * -------------
  * This class contains no mutable shared state and performs no hardware
  * access. Thread safety depends entirely on the derived implementation.
  */

namespace attenuation
{
    class AttenuatorBase : public IAttenuator
    {
    public:

        /**
         * Determines whether the specified attenuation is supported.
         *
         * The value is considered valid when it lies within the supported
         * attenuation range reported by the derived class.
         *
         * @param attenuationDb Attenuation to validate.
         *
         * @return True if the attenuation is within the supported range.
         */
        [[nodiscard]]
        bool isValidAttenuation(const double attenuationDb) const override
        {
            return attenuationDb >= getMinimumAttenuation()
                && attenuationDb <= getMaximumAttenuation();
        }

        /**
         * Returns the closest attenuation supported by the hardware.
         *
         * The requested value is first clamped into the supported range.
         * If the attenuator has a programmable step size, the value is then
         * rounded to the nearest supported attenuation level.
         *
         * Fixed attenuators report a step size of 0.0, therefore the only
         * possible attenuation is the fixed hardware attenuation.
         *
         * The hardware state is not modified.
         *
         * @param attenuationDb Requested attenuation.
         *
         * @return Quantized attenuation.
         */
        [[nodiscard]]
        double quantizeAttenuation(double attenuationDb) const override
        {
            const double minimum = getMinimumAttenuation();
            const double maximum = getMaximumAttenuation();
            const double step = getStepSize();

            attenuationDb = std::clamp(attenuationDb, minimum, maximum);
            if (step == 0.0) {
                return minimum;
            }

            return minimum+ std::round((attenuationDb - minimum) / step) * step;
        }

    protected:

        ~AttenuatorBase() override = default;
    };

}

#endif //CPPFIRMWAREEMBEDDED_ATTENUATORBASE_HPP
