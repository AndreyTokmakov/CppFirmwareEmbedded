/**============================================================================
Name        : PE43711.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE43711.cpp
============================================================================**/

#include "PE43711.hpp"

/******************************************************************************
 *
 *  PE43711.cpp
 *
 *  Implementation of the pSemi PE43711 digital step attenuator.
 *
 *  Overview
 *  --------
 *
 *  The PE43711 is a high-linearity RF digital step attenuator controlled
 *  through a serial SPI-compatible interface.
 *
 *  This class implements only the PE43711-specific functionality:
 *
 *      • attenuation range
 *      • attenuation resolution
 *      • attenuation register encoding
 *      • SPI transfer word formatting
 *
 *  SPI communication itself is implemented by SpiAttenuator.
 *
 ******************************************************************************/


namespace
{
    constexpr double kMinimumAttenuationDb = 0.0;
    constexpr double kMaximumAttenuationDb = 31.75;
    constexpr double kStepSizeDb = 0.25;
    constexpr uint16_t kAttenuationMask = 0x007F;
}

namespace attenuation
{
    PE43711::PE43711(hal::ISpiBus& spi) : SpiAttenuator(spi)
    {
    }

    std::string PE43711::getName() const
    {
        return "PE43711";
    }

    double PE43711::getMinimumAttenuation() const
    {
        return kMinimumAttenuationDb;
    }

    double PE43711::getMaximumAttenuation() const
    {
        return kMaximumAttenuationDb;
    }

    double PE43711::getStepSize() const
    {
        return kStepSizeDb;
    }

    uint16_t PE43711::encodeAttenuation(double attenuationDb) const
    {
        // PE43711 attenuation encoding:
        //
        //     Code = Attenuation / 0.25 dB
        //
        // Examples:
        //
        //     0.00 dB  -> 0
        //     0.25 dB  -> 1
        //     10.00 dB -> 40
        //     31.75 dB -> 127
        //
        return static_cast<uint16_t>(attenuationDb / kStepSizeDb);
    }

    uint16_t PE43711::buildSpiWord(const uint16_t value) const
    {
        // PE43711 uses a 16-bit SPI transfer word.
        //
        // The attenuation register occupies the lower 7 bits.
        //
        // Bit layout:
        //
        //     [15 ........ 7][6 ...... 0]
        //          0          ATT[6:0]
        //
        return value & kAttenuationMask;
    }

} // namespace attenuation