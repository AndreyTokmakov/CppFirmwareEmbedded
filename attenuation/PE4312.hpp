/**============================================================================
Name        : PE4312.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE4312.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_PE4312_HPP
#define CPPFIRMWAREEMBEDDED_PE4312_HPP

#include <cstdint>

#include "SpiAttenuator.hpp"

namespace attenuation
{
    /**
     * Driver model for the pSemi PE4312 digital step attenuator.
     *
     * Overview
     * --------
     *
     * The PE4312 is a broadband RF digital step attenuator controlled through a
     * serial SPI-compatible interface.
     *
     * The device provides programmable attenuation from 0.0 dB to 31.5 dB with a
     * resolution of 0.5 dB.
     *
     * The PE4312 is commonly used in:
     *     • RF receivers
     *     • RF transmitters
     *     • Automatic gain control systems
     *     • Software defined radios
     *     • RF test equipment
     *
     * Device characteristics
     * ----------------------
     *
     *     Attenuation range:
     *         0.0 dB ... 31.5 dB
     *
     *     Resolution:
     *         0.5 dB
     *
     *     Control interface:
     *         SPI-compatible serial interface
     *
     *     Register width:
     *         6 bits
     *
     * Architecture
     * ------------
     *
     * PE4312 contains only device-specific logic.
     *
     * SPI communication is implemented by SpiAttenuator:
     *
     *     PE4312
     *        |
     *        v
     *     SpiAttenuator
     *        |
     *        v
     *     ISpiBus
     *
     *
     * PE4312 responsibilities:
     *
     *     • attenuation limits
     *     • attenuation step size
     *     • attenuation encoding
     *     • SPI frame formatting
     *
     *
     * SpiAttenuator responsibilities:
     *
     *     • SPI transfer
     *     • SPI configuration
     *
     *
     * Register encoding
     * -----------------
     *
     * PE4312 uses a 6-bit binary attenuation value:
     *
     *     attenuationCode = attenuation / 0.5
     *
     * Examples:
     *
     *     0.0 dB:
     *         000000
     *     0.5 dB:
     *         000001
     *     10.0 dB:
     *         010100
     *     31.5 dB:
     *         111111
     */
    class PE4312 : public SpiAttenuator
    {
    public:

        /**
         * Creates a PE4312 attenuator driver.
         * @param spi SPI bus used for communication.
         */
        explicit PE4312(hal::ISpiBus& spi);

        /**
         * Returns device name.
         * @return "PE4312".
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
         * @return 31.5 dB.
         */
        [[nodiscard]]
        double getMaximumAttenuation() const override;

        /**
         * Returns attenuation step size.
         * @return 0.5 dB.
         */
        [[nodiscard]]
        double getStepSize() const override;


    protected:

        /**
         * Converts attenuation value into PE4312 register format.
         *
         * PE4312 attenuation encoding:
         *     code = attenuation / 0.5
         *
         * Examples:
         *
         *     0.0 dB  -> 0
         *     0.5 dB  -> 1
         *     10.0 dB -> 20
         *     31.5 dB -> 63
         *
         * @param attenuationDb Quantized attenuation value.
         *
         * @return Six-bit attenuation register.
         */
        [[nodiscard]]
        uint16_t encodeAttenuation(double attenuationDb) const override;

        /**
         * Builds PE4312 SPI transfer word.
         *
         * PE4312 uses a 16-bit SPI frame where the attenuation bits occupy the
         * lower part of the transfer word.
         *
         * @param value Encoded attenuation register.
         * @return SPI transfer word.
         */
        [[nodiscard]]
        uint16_t buildSpiWord(uint16_t value) const override;

    private:

        static constexpr double MinimumAttenuationDb = 0.0;
        static constexpr double MaximumAttenuationDb = 31.5;
        static constexpr double StepSizeDb = 0.5;
    };
}

#endif //CPPFIRMWAREEMBEDDED_PE4312_HPP
