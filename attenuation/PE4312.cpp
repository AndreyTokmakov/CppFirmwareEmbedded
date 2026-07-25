/**============================================================================
Name        : PE4312.cpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE4312.cpp
============================================================================**/

#include "PE4312.hpp"

namespace attenuation
{
    namespace
    {
        constexpr double kMinimumAttenuation = 0.0;
        constexpr double kMaximumAttenuation = 31.5;
        constexpr double kStepSize = 0.5;
    }

    PE4312::PE4312(): DigitalStepAttenuator(kMinimumAttenuation, kMaximumAttenuation, kStepSize){
    }

    std::string PE4312::GetName() const
    {
        return "pSemi PE4312";
    }

    uint16_t PE4312::EncodeAttenuation(const double attenuationDb) const
    {
        return static_cast<uint16_t>(attenuationDb / GetStepSize());
    }
}