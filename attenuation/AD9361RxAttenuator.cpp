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
    AD9361RxAttenuator::AD9361RxAttenuator(IAd9361& device) : device(device)
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

    Error AD9361RxAttenuator::SetAttenuation(const double attenuationDb)
    {
        if (!IsValidAttenuation(attenuationDb)) {
            return Error::InvalidParameter;
        }

        pendingAttenuation = QuantizeAttenuation(attenuationDb);
        dirty = true;

        return Error::None;
    }

    double AD9361RxAttenuator::GetAttenuation() const
    {
        return currentAttenuation;
    }

    double AD9361RxAttenuator::GetMinimumAttenuation() const
    {
        return 0.0;
    }

    double AD9361RxAttenuator::GetMaximumAttenuation() const
    {
        return 89.75;
    }

    double AD9361RxAttenuator::GetStepSize() const
    {
        return 0.25;
    }

    Error AD9361RxAttenuator::Apply()
    {
        if (!dirty) {
            return Error::None;
        }

        busy = true;
        const Error error = device.SetRxAttenuation(pendingAttenuation);

        busy = false;
        if (error != Error::None) {
            return error;
        }

        currentAttenuation = pendingAttenuation;
        dirty = false;

        return Error::None;
    }

    bool AD9361RxAttenuator::IsBusy() const
    {
        return busy;
    }

    Error AD9361RxAttenuator::Reset()
    {
        pendingAttenuation = 0.0;
        dirty = true;

        return Apply();
    }

}