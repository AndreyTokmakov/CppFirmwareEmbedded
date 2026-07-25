/**============================================================================
Name        : AD9361RxAttenuator.cpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : AD9361RxAttenuator.cpp
============================================================================**/

#include "AD9361RxAttenuator.hpp"

namespace rf
{
    AD9361RxAttenuator::AD9361RxAttenuator(IAd9361RxGainControl& channel): channel(channel)
    {
    }

    std::string AD9361RxAttenuator::GetName() const
    {
        return "Analog Devices AD9361 RX";
    }

    bool AD9361RxAttenuator::IsProgrammable() const
    {
        return true;
    }

    Error AD9361RxAttenuator::SetAttenuation(double attenuationDb)
    {
        if (!IsValidAttenuation(attenuationDb))
        {
            return Error::InvalidParameter;
        }

        pendingAttenuation = QuantizeAttenuation(attenuationDb);
        configurationDirty = true;

        return Error::None;
    }

    double AD9361RxAttenuator::GetAttenuation() const
    {
        return appliedAttenuation;
    }

    double AD9361RxAttenuator::GetMinimumAttenuation() const
    {
        return MinimumAttenuation;
    }

    double AD9361RxAttenuator::GetMaximumAttenuation() const
    {
        return MaximumAttenuation;
    }

    double AD9361RxAttenuator::GetStepSize() const
    {
        return StepSize;
    }

    Error AD9361RxAttenuator::Apply()
    {
        if (!configurationDirty)
        {
            return Error::None;
        }

        updateInProgress = true;

        const Error error = channel.SetRxAttenuation(pendingAttenuation);

        updateInProgress = false;

        if (error != Error::None)
        {
            return error;
        }

        appliedAttenuation = pendingAttenuation;
        configurationDirty = false;

        return Error::None;
    }

    bool AD9361RxAttenuator::IsBusy() const
    {
        return updateInProgress;
    }

    Error AD9361RxAttenuator::Reset()
    {
        pendingAttenuation = MinimumAttenuation;
        configurationDirty = true;

        return Apply();
    }
}