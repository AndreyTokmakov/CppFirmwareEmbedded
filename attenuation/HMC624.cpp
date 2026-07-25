/**============================================================================
Name        : HMC624.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : HMC624.cpp
============================================================================**/

#include "HMC624.hpp"
#include "HMC624.hpp"

namespace rf
{
    namespace
    {
        constexpr double kMinimumAttenuation = 0.0;
        constexpr double kMaximumAttenuation = 31.0;
        constexpr double kStepSize = 0.5;
    }

    HMC624::HMC624()
        : DigitalStepAttenuator(
              kMinimumAttenuation,
              kMaximumAttenuation,
              kStepSize)
    {
    }

    std::string HMC624::GetName() const
    {
        return "Analog Devices HMC624";
    }

    uint16_t HMC624::EncodeAttenuation(const double attenuationDb) const
    {   /**
         * Convert attenuation value into device steps.
         * Example:
         *  attenuationDb = 12.5
         *  12.5 / 0.5 = 25
         *  Binary:
         *      25 = 0b011001
        **/
        return static_cast<uint16_t>(attenuationDb / GetStepSize());
    }

    uint16_t HMC624::BuildControlWord(const uint16_t attenuationCode) const
    {   /*
         * HMC624 uses:
         * Bit 0..5:
         *   attenuation value
         * Bit 6:
         *   latch/update flag
         *  Example:
         *      attenuationCode = 0x15
         *      Result:
         *          0b010101
         *      becomes:
         *           0b010101 + control bit
         */
        constexpr uint16_t UpdateBit = 1 << 6;

        return attenuationCode | UpdateBit;
    }
}
