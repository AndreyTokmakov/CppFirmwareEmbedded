/**============================================================================
Name        : DigitalStepAttenuator.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : DigitalStepAttenuator.cpp
============================================================================**/

#include "DigitalStepAttenuator.hpp"

namespace rf
{
    DigitalStepAttenuator::DigitalStepAttenuator(const double minimumAttenuation,
                                                 const double maximumAttenuation,
                                                 const double stepSize):
        minimumAttenuation(minimumAttenuation),
        maximumAttenuation(maximumAttenuation),
        stepSize(stepSize)
    {
    }

    bool DigitalStepAttenuator::IsProgrammable() const
    {
        return true;
    }

    Error DigitalStepAttenuator::SetAttenuation(double attenuationDb)
    {
        if (!IsValidAttenuation(attenuationDb))
        {
            return Error::InvalidParameter;
        }

        pendingAttenuation = QuantizeAttenuation(attenuationDb);
        configurationDirty = true;

        return Error::None;
    }

    double DigitalStepAttenuator::GetAttenuation() const
    {
        return appliedAttenuation;
    }

    double DigitalStepAttenuator::GetMinimumAttenuation() const
    {
        return minimumAttenuation;
    }

    double DigitalStepAttenuator::GetMaximumAttenuation() const
    {
        return maximumAttenuation;
    }

    double DigitalStepAttenuator::GetStepSize() const
    {
        return stepSize;
    }

    Error DigitalStepAttenuator::Apply()
    {
        if (!configurationDirty)
        {
            return Error::None;
        }

        updateInProgress = true;

        const uint16_t attenuationCode = EncodeAttenuation(pendingAttenuation);
        const uint16_t controlWord = BuildControlWord(attenuationCode);

        const Error error = ProgramControlWord(controlWord);

        updateInProgress = false;

        if (error != Error::None)
        {
            return error;
        }

        appliedAttenuation = pendingAttenuation;
        configurationDirty = false;

        return Error::None;
    }

    bool DigitalStepAttenuator::IsBusy() const
    {
        return updateInProgress;
    }

    Error DigitalStepAttenuator::Reset()
    {
        pendingAttenuation = minimumAttenuation;
        configurationDirty = true;

        return Apply();
    }

    uint16_t DigitalStepAttenuator::BuildControlWord(const uint16_t attenuationCode) const
    {
        return attenuationCode;
    }
}