/**============================================================================
Name        : HMC624.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : HMC624.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_HMC624_HPP
#define CPPFIRMWAREEMBEDDED_HMC624_HPP

#pragma once

#include "DigitalStepAttenuator.hpp"

namespace attenuation
{
    /**
     * Represents the Analog Devices HMC624 digital step attenuator.
     *
     * The HMC624 is a broadband digitally controlled RF attenuator.
     * The device provides programmable attenuation using a 6-bit control
     * word.
     *
     * Device characteristics:
     *   - Attenuation range: 0.0 dB to 31.0 dB
     *   - Resolution: 0.5 dB
     *   - 6-bit attenuation control
     *
     * The class implements only HMC624-specific encoding and hardware
     * programming logic. Generic attenuation handling is provided by
     * DigitalStepAttenuator.
     */
    class HMC624 : public DigitalStepAttenuator
    {
    public:

        /**
         * Creates a new HMC624 driver instance.
         */
        HMC624();

        /**
         * Returns device name.
         *
         * @return Device name.
         */
        [[nodiscard]]
        std::string GetName() const override;

    protected:

        /**
         * Converts attenuation in dB into the HMC624 attenuation code.
         *
         * HMC624 uses a linear binary attenuation code where each bit
         * represents a 0.5 dB attenuation step.
         *
         * Example:
         *   0.0 dB  -> 0x00
         *   0.5 dB  -> 0x01
         *   10.0 dB -> 0x14
         *
         * @param attenuationDb Attenuation in dB.
         *
         * @return 6-bit attenuation code.
         */
        [[nodiscard]]
        uint16_t EncodeAttenuation(double attenuationDb) const override;

        /**
         * Adds HMC624 specific control bits.
         *
         * The HMC624 requires the attenuation code to be placed into
         * the lower bits of the control word.
         *
         * @param attenuationCode Encoded attenuation value.
         *
         * @return Complete HMC624 control word.
         */
        [[nodiscard]]
        uint16_t BuildControlWord(uint16_t attenuationCode) const override;

        /**
         * Sends control word to the physical device.
         *
         * The implementation depends on the hardware platform:
         * SPI, GPIO, FPGA interface, etc.
         *
         * @param controlWord HMC624 control word.
         *
         * @return Operation result.
         */
        Error ProgramControlWord(uint16_t controlWord) override = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_HMC624_HPP
