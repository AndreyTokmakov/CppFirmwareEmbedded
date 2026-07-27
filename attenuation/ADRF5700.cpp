/**============================================================================
Name        : ADRF5700.cpp
Created on  : 27.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : ADRF5700.cpp
============================================================================**/

#include "ADRF5700.hpp"

/******************************************************************************
 *
 *  ADRF5700.cpp
 *
 *  Implementation of the Analog Devices ADRF5700 digital step attenuator.
 *
 *  Overview
 *  --------
 *
 *  The ADRF5700 is a high-performance RF digital step attenuator controlled
 *  through an SPI-compatible serial interface.
 *
 *  This class implements only the ADRF5700-specific functionality:
 *
 *      • attenuation range
 *      • attenuation resolution
 *      • attenuation register encoding
 *      • SPI transfer word formatting
 *
 *  SPI communication itself is implemented by SpiAttenuator.
 *
 *
 *  Architecture:
 *
 *      ADRF5700
 *           |
 *           v
 *      SpiAttenuator
 *           |
 *           v
 *        ISpiBus
 *
 ******************************************************************************/

namespace
{
    constexpr double kMinimumAttenuationDb = 0.0;
    constexpr double kMaximumAttenuationDb = 31.5;
    constexpr double kStepSizeDb = 0.5;

    /**
     * ADRF5700 attenuation field width.
     *
     * The attenuation value is represented by 6 bits:
     *
     *     0b000000 ... 0b111111
     *
     * corresponding to:
     *
     *     0.0 dB ... 31.5 dB
     */
    constexpr uint16_t kAttenuationMask = 0x003F;

    /**
     * Default control bits.
     *
     * Reserved and configuration bits are kept at their default state.
     *
     * Device-specific features can be added here later:
     *
     *     • bypass mode
     *     • enable control
     *     • operating modes
     */
    constexpr uint16_t kDefaultControlBits = 0x0000;
}

namespace attenuation
{
    ADRF5700::ADRF5700(hal::ISpiBus& spi): SpiAttenuator(spi)
    {
    }

    std::string ADRF5700::getName() const
    {
        return "ADRF5700";
    }

    double ADRF5700::getMinimumAttenuation() const
    {
        return kMinimumAttenuationDb;
    }

    double ADRF5700::getMaximumAttenuation() const
    {
        return kMaximumAttenuationDb;
    }

    double ADRF5700::getStepSize() const
    {
        return kStepSizeDb;
    }

    uint16_t ADRF5700::encodeAttenuation(const double attenuationDb) const
    {
        //
        // ADRF5700 attenuation encoding:
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

    uint16_t ADRF5700::buildSpiWord(const uint16_t value) const
    {
        //
        // ADRF5700 SPI transfer word.
        //
        // Current layout:
        //
        //     [15 ........ 6][5 ..... 0]
        //        control       ATT[5:0]
        //
        // The attenuation field occupies the lower 6 bits.
        //
        // Additional device control bits can be added without changing the
        // SpiAttenuator implementation.
        //
        return kDefaultControlBits | (value & kAttenuationMask);
    }

} // namespace attenuation