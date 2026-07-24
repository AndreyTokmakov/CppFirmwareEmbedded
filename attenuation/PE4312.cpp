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
        if (!IsValidAttenuation(attenuationDb)){
            return Error::InvalidParameter;
        }

        pendingAttenuation = QuantizeAttenuation(attenuationDb);
        dirty = true;
        return Error::None;
    }

    double PE4312::GetAttenuation() const
    {
        return currentAttenuation;
    }

    double PE4312::GetMinimumAttenuation() const
    {
        return 0.0;
    }

    double PE4312::GetMaximumAttenuation() const
    {
        return 31.5;
    }

    double PE4312::GetStepSize() const
    {
        return 0.5;
    }

    Error PE4312::Apply()
    {
        if (!dirty){
            return Error::None;
        }

        busy = true;
        const Error error = WriteRegister(EncodeAttenuation(pendingAttenuation));

        busy = false;
        if (error != Error::None){
            return error;
        }

        currentAttenuation = pendingAttenuation;
        dirty = false;

        return Error::None;
    }

    bool PE4312::IsBusy() const
    {
        return busy;
    }

    Error PE4312::Reset()
    {
        pendingAttenuation = 0.0;
        dirty = true;

        return Apply();
    }

    uint8_t PE4312::EncodeAttenuation(const double attenuationDb)
    {
        return static_cast<uint8_t>(attenuationDb / 0.5);
    }
}