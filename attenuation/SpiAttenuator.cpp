/**============================================================================
Name        : SpiAttenuator.cpp
Created on  : 27.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : SpiAttenuator.cpp
============================================================================**/

#include "SpiAttenuator.hpp"

/******************************************************************************
 *
 *  SpiAttenuator.cpp
 *
 *  Common implementation for SPI-controlled RF digital step attenuators.
 *
 *  Overview
 *  --------
 *
 *  Many RF digital attenuators communicate through an SPI-compatible serial
 *  interface. Although individual devices use different register layouts, the
 *  communication sequence is largely identical:
 *
 *      Requested attenuation
 *              │
 *              ▼
 *      Encode attenuation register
 *              │
 *              ▼
 *      Build SPI transfer word
 *              │
 *              ▼
 *      Configure SPI bus
 *              │
 *              ▼
 *      Transfer SPI word
 *
 *  This class implements the SPI communication layer shared by multiple
 *  attenuator families. Device-specific drivers only provide the attenuation
 *  register encoding and the SPI word layout.
 *
 ******************************************************************************/

namespace attenuation
{
    SpiAttenuator::SpiAttenuator(hal::ISpiBus& spiBus) : spi(spiBus)
    {
    }

    Error SpiAttenuator::writeAttenuationRegister(const uint16_t value)
    {
        const uint16_t spiWord = buildSpiWord(value);

        const hal::SpiMode previousMode = spi.getMode();
        const uint32_t previousFrequency = spi.getFrequency();

        Error result = spi.setMode(getSpiMode());
        if (result != Error::Success)
        {
            return result;
        }

        result = spi.setFrequency(getSpiFrequency());
        if (result != Error::Success)
        {
            spi.setMode(previousMode);
            return result;
        }

        result = spi.write(spiWord);

        spi.setFrequency(previousFrequency);
        spi.setMode(previousMode);

        return result;
    }

    hal::SpiMode SpiAttenuator::getSpiMode() const
    {
        return hal::SpiMode::Mode0;
    }

    uint32_t SpiAttenuator::getSpiFrequency() const
    {
        //
        // Most RF attenuators support SPI clock frequencies significantly higher
        // than 1 MHz. A conservative default is provided here to maximize
        // compatibility. Individual drivers may override this value with the
        // maximum frequency recommended by the device datasheet.
        //
        return 1'000'000;
    }

}