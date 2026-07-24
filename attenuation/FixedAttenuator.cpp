/**============================================================================
Name        : FixedAttenuator.cpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : FixedAttenuator.cpp
============================================================================**/

#include "FixedAttenuator.hpp"

namespace rf
{
    FixedAttenuator::FixedAttenuator(const double attenuationDb) : attenuation(attenuationDb)
    {
    }

    std::string FixedAttenuator::GetName() const
    {
        return "Fixed Attenuator";
    }

    bool FixedAttenuator::IsProgrammable() const
    {
        return false;
    }

    Error FixedAttenuator::SetAttenuation(double)
    {
        return Error::NotSupported;
    }

    double FixedAttenuator::GetAttenuation() const
    {
        return attenuation;
    }

    double FixedAttenuator::GetMinimumAttenuation() const
    {
        return attenuation;
    }

    double FixedAttenuator::GetMaximumAttenuation() const
    {
        return attenuation;
    }

    double FixedAttenuator::GetStepSize() const
    {
        return 0.0;
    }

    Error FixedAttenuator::Reset()
    {
        return Error::None;
    }
}