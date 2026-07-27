/**============================================================================
Name        : PE43711.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE43711.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_PE43711_HPP
#define CPPFIRMWAREEMBEDDED_PE43711_HPP

#include "SpiAttenuator.hpp"


/**
 * Driver model for the pSemi PE43711 digital step attenuator.
 *
 * Overview
 * --------
 *
 * The PE43711 is a high-linearity RF digital step attenuator controlled through
 * a serial SPI-compatible interface.
 *
 * The device provides programmable attenuation from 0.0 dB to 31.75 dB with a
 * resolution of 0.25 dB.
 *
 * The PE43711 is commonly used in:
 *
 *     • RF receivers
 *     • RF transmitters
 *     • Software defined radios
 *     • Automatic gain control systems
 *     • RF test and measurement equipment
 *
 *
 * Device characteristics
 * ----------------------
 *
 *     Attenuation range:
 *         0.0 dB ... 31.75 dB
 *
 *     Resolution:
 *         0.25 dB
 *
 *     Control interface:
 *         SPI-compatible serial interface
 *
 *     Register width:
 *         7 bits
 *
 *
 * Architecture
 * ------------
 *
 * PE43711 contains only the device-specific logic.
 *
 * SPI communication is implemented by SpiAttenuator:
 *
 *
 *     PE43711
 *        |
 *        v
 *     SpiAttenuator
 *        |
 *        v
 *     ISpiBus
 *
 *
 * PE43711 responsibilities:
 *
 *     • attenuation limits
 *     • attenuation step size
 *     • attenuation register encoding
 *     • SPI transfer word formatting
 *
 *
 * SpiAttenuator responsibilities:
 *
 *     • SPI transfer
 *     • SPI bus access
 *     • SPI configuration
 *
 *
 * Register encoding
 * -----------------
 *
 * The PE43711 uses a 7-bit attenuation register.
 *
 * Encoding:
 *
 *     attenuationCode = attenuation / 0.25
 *
 *
 * Examples:
 *
 *     0.00 dB:
 *         0000000
 *
 *     0.25 dB:
 *         0000001
 *
 *     10.00 dB:
 *         0101000
 *
 *     31.75 dB:
 *         1111111
 *
 */

namespace attenuation
{
    class PE43711 : public SpiAttenuator
    {
    public:
        /**
         * Creates a PE43711 attenuator driver.
         * @param spi SPI bus used for communication.
         */
        explicit PE43711(hal::ISpiBus& spi);

        /**
         * Returns device name.
         * @return "PE43711".
         */
        [[nodiscard]]
        std::string getName() const override;

        /**
         * Returns minimum supported attenuation.
         * @return 0.0 dB.
         */
        [[nodiscard]]
        double getMinimumAttenuation() const override;

        /**
         * Returns maximum supported attenuation.
         * @return 31.75 dB.
         */
        [[nodiscard]]
        double getMaximumAttenuation() const override;

        /**
         * Returns attenuation step size.
         * @return 0.25 dB.
         */
        [[nodiscard]]
        double getStepSize() const override;


    protected:

        /**
         * Converts attenuation value into PE43711 register format.
         * Encoding:
         *     code = attenuation / 0.25
         *
         * Examples:
         *     0.00 dB  -> 0
         *     0.25 dB  -> 1
         *     10.00 dB -> 40
         *     31.75 dB -> 127
         *
         * @param attenuationDb Quantized attenuation value.
         * @return Seven-bit attenuation register.
         */
        [[nodiscard]]
        uint16_t encodeAttenuation(double attenuationDb) const override;


        /**
         * Builds PE43711 SPI transfer word.
         *
         * The PE43711 uses a 16-bit SPI transfer frame. The attenuation register
         * occupies the lower seven bits of the transfer word.
         *
         * @param value Encoded attenuation register.
         * @return SPI transfer word.
         */
        [[nodiscard]]
        uint16_t buildSpiWord(uint16_t value) const override;


    private:

        static constexpr double MinimumAttenuationDb = 0.0;
        static constexpr double MaximumAttenuationDb = 31.75;
        static constexpr double StepSizeDb = 0.25;
    };

} // namespace attenuation

#endif //CPPFIRMWAREEMBEDDED_PE43711_HPP
