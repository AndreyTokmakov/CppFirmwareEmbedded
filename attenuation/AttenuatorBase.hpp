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

namespace rf
{

    /**
     * @brief Base implementation shared by most attenuators.
     *
     * Provides common implementations for device state management
     * and attenuation validation.
     */
    class AttenuatorBase : public IAttenuator
    {
    public:

        Error Enable() override
        {
            enabled = true;
            return Error::None;
        }

        Error Disable() override
        {
            enabled = false;
            return Error::None;
        }

        bool IsEnabled() const override
        {
            return enabled;
        }

        Error Apply() override
        {
            return Error::None;
        }

        bool IsBusy() const override
        {
            return false;
        }

        bool IsValidAttenuation(double attenuationDb) const override
        {
            return
                attenuationDb >= GetMinimumAttenuation() &&
                attenuationDb <= GetMaximumAttenuation();
        }

        double QuantizeAttenuation(double attenuationDb) const override
        {
            const double min = GetMinimumAttenuation();
            const double max = GetMaximumAttenuation();
            const double step = GetStepSize();

            attenuationDb = std::clamp(attenuationDb, min, max);

            if (step == 0.0)
            {
                return min;
            }

            return min + std::round((attenuationDb - min) / step) * step;
        }

    protected:

        ~AttenuatorBase() override = default;

    private:

        bool enabled = true;
    };
}

#endif //CPPFIRMWAREEMBEDDED_ATTENUATORBASE_HPP
