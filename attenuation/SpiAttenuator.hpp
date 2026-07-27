/**============================================================================
Name        : SpiAttenuator.hpp
Created on  : 27.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : SpiAttenuator.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_SPIATTENUATOR_HPP
#define CPPFIRMWAREEMBEDDED_SPIATTENUATOR_HPP

#include <cstdint>
#include "DigitalStepAttenuator.hpp"
#include "ISpiBus.hpp"

/**
* Base class for SPI-controlled RF digital step attenuators.
*
* Overview
* --------
*
* Many RF digital step attenuators communicate through an SPI-compatible serial
* interface. Although individual devices use different register layouts, the
* communication sequence is almost identical:
*
*     Requested attenuation
*             ↓
*     Encode attenuation register
*             ↓
*     Build SPI transfer word
*             ↓
*     Transfer over SPI
*
* This class implements the common SPI communication layer shared by multiple
* attenuator families. Derived classes describe only the device-specific
* register layout and attenuation characteristics.
*
* Typical devices include:
*
*     • pSemi PE4312
*     • pSemi PE43711
*
* Responsibilities
* ----------------
*
* This class performs:
*
*     • SPI communication
*     • transmission of the encoded attenuation register
*
* Derived classes provide:
*
*     • attenuation encoding
*     • SPI transfer word layout
*
* Thread Safety
* -------------
*
* This class is not internally synchronized.
*/


namespace attenuation
{
    class SpiAttenuator : public DigitalStepAttenuator
    {
    public:

        /**
         * Creates an SPI-controlled attenuator.
         *
         * @param spi SPI bus used to communicate with the device.
         */
        explicit SpiAttenuator(hal::ISpiBus& spi);

    protected:

        /**
         * Writes an encoded attenuation register to the hardware.
         *
         * The register is first converted into a device-specific SPI transfer word
         * and then transmitted over the SPI bus.
         *
         * @param value Encoded attenuation register.
         * @return Operation status.
         */
        Error writeAttenuationRegister(uint16_t value) final;

        /**
         * Builds the SPI transfer word required by the device.
         *
         * Different attenuator families use different bit assignments inside the
         * SPI transfer word. The returned value contains the complete word exactly
         * as it should appear on the SPI bus.
         *
         * Examples:
         *     PE4312:
         *         16-bit SPI word
         *     PE43711:
         *         16-bit SPI word with a different register layout
         *
         * @param value Encoded attenuation register.
         * @return Complete SPI transfer word.
         */
        [[nodiscard]]
        virtual uint16_t buildSpiWord(uint16_t value) const = 0;

        /**
         * Returns the SPI mode required by the device.
         *
         * Most RF attenuators operate using SPI Mode 0, therefore the default
         * implementation returns Mode0.
         *
         * Derived classes may override this method if another SPI mode is required.
         *
         * @return SPI mode.
         */
        [[nodiscard]]
          virtual hal::SpiMode getSpiMode() const;

        /**
         * Returns the SPI clock frequency.
         *
         * The default implementation returns 1 MHz. Device-specific drivers may
         * override this method if another frequency is recommended by the
         * datasheet.
         *
         * @return SPI clock frequency in hertz.
         */
        [[nodiscard]]
        virtual uint32_t getSpiFrequency() const;


    private:

        hal::ISpiBus& spi;
    };

}

#endif //CPPFIRMWAREEMBEDDED_SPIATTENUATOR_HPP
