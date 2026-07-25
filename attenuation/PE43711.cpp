/**============================================================================
Name        : PE43711.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE43711.cpp
============================================================================**/

#include "PE43711.hpp"

#include "PE43711.hpp"

namespace attenuation
{
    namespace
    {
        constexpr double kMinimumAttenuation = 0.0;
        constexpr double kMaximumAttenuation = 31.75;
        constexpr double kStepSize = 0.25;
    }

    PE43711::PE43711(): DigitalStepAttenuator(kMinimumAttenuation, kMaximumAttenuation, kStepSize)
    {
    }

    std::string PE43711::GetName() const
    {
        return "pSemi PE43711";
    }

    uint16_t PE43711::EncodeAttenuation(const double attenuationDb) const
    {
        return static_cast<uint16_t>(attenuationDb / GetStepSize());
    }

}