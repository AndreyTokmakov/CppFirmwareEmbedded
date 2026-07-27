/**============================================================================
Name        : PE4312.cpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE4312.cpp
============================================================================**/

#include "PE4312.hpp"

/******************************************************************************
 *
 *  PE4312.cpp
 *
 *  Implementation of the pSemi PE4312 digital step attenuator.
 *
 *  Overview
 *  --------
 *
 *  The PE4312 is a broadband RF digital step attenuator controlled through a
 *  serial SPI-compatible interface.
 *
 *  This class implements only the PE4312-specific behavior:
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
    constexpr double kMaximumAttenuationDb = 31.5;
    constexpr double kStepSizeDb = 0.5;
}

namespace attenuation
{
    PE4312::PE4312(hal::ISpiBus& spi) : SpiAttenuator(spi)
    {
    }

    std::string PE4312::getName() const {
        return "PE4312";
    }

    double PE4312::getMinimumAttenuation() const {
        return kMinimumAttenuationDb;
    }

    double PE4312::getMaximumAttenuation() const {
        return kMaximumAttenuationDb;
    }

    double PE4312::getStepSize() const {
        return kStepSizeDb;
    }

    uint16_t PE4312::encodeAttenuation(const double attenuationDb) const
    {
        //
        // PE4312 attenuation code:
        //
        //     Code = Attenuation / 0.5 dB
        //
        // Examples:
        //
        //     0.0 dB  -> 0
        //     0.5 dB  -> 1
        //     10.0 dB -> 20
        //     31.5 dB -> 63
        //
        return static_cast<uint16_t>(attenuationDb / kStepSizeDb);
    }

    uint16_t PE4312::buildSpiWord(const uint16_t value) const
    {
        //
        // PE4312 uses a 16-bit SPI transfer.
        //
        // The attenuation value occupies the lower 6 bits.
        //
        // Bit layout:
        //
        //     [15 ........ 6][5 ..... 0]
        //          0          ATT[5:0]
        //
        return value & 0x003F;
    }
}