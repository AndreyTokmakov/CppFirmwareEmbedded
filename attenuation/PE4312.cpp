/**============================================================================
Name        : PE4312.cpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE4312.cpp
============================================================================**/

#include "PE4312.hpp"

#include "PE4312.hpp"

namespace rf
{
    PE4312::PE4312() = default;

    std::string PE4312::GetName() const
    {
        return "pSemi PE4312";
    }

    bool PE4312::IsProgrammable() const
    {
        return true;
    }

    Error PE4312::SetAttenuation(const double attenuationDb)
    {
        if (!IsValidAttenuation(attenuationDb))
        {
            return Error::InvalidParameter;
        }

        pendingAttenuation = QuantizeAttenuation(attenuationDb);
        configurationDirty = true;

        return Error::None;
    }

    double PE4312::GetAttenuation() const
    {
        return appliedAttenuation;
    }

    double PE4312::GetMinimumAttenuation() const
    {
        return MinimumAttenuation;
    }


    double PE4312::GetMaximumAttenuation() const
    {
        return MaximumAttenuation;
    }

    double PE4312::GetStepSize() const
    {
        return StepSize;
    }

    Error PE4312::Apply()
    {
        if (!configurationDirty){
            return Error::None;
        }

        updateInProgress = true;

        const uint8_t controlWord = BuildControlWord(pendingAttenuation);
        const Error error = WriteAttenuationCode(controlWord);

        updateInProgress = false;
        if (error != Error::None) {
            return error;
        }

        appliedAttenuation = pendingAttenuation;
        configurationDirty = false;

        return Error::None;
    }

    bool PE4312::IsBusy() const
    {
        return updateInProgress;
    }

    Error PE4312::Reset()
    {
        pendingAttenuation = 0.0;
        configurationDirty = true;

        return Apply();
    }

    uint8_t PE4312::BuildControlWord(double attenuationDb)
    {
        return static_cast<uint8_t>(attenuationDb / StepSize);
    }
}