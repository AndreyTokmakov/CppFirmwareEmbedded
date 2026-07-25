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

#include "DigitalStepAttenuator.hpp"

namespace attenuation
{
    /**
     * Represents the pSemi PE4312 digital step attenuator.
     *
     * The PE4312 is a digitally controlled RF step attenuator providing
     * accurate and repeatable RF signal level adjustment over a wide
     * frequency range. The device is programmed through a serial interface
     * using a 6-bit attenuation code.
     *
     * Device characteristics:
     *   - Attenuation range: 0.0 dB to 31.5 dB
     *   - Resolution: 0.5 dB
     *   - 6-bit attenuation code
     *   - Low insertion loss
     *   - High attenuation accuracy
     *   - Operating frequency from DC to several GHz
     *
     * Typical applications:
     *   - Software Defined Radio (SDR)
     *   - RF front-end gain control
     *   - Automatic Gain Control (AGC)
     *   - Test and measurement equipment
     *   - Radar systems
     *   - Wireless communication systems
     *
     * This class implements only the PE4312-specific functionality.
     * All common programmable attenuator logic, including attenuation
     * validation, quantization, deferred configuration updates and state
     * management, is implemented by DigitalStepAttenuator.
     */
    class PE4312 : public DigitalStepAttenuator
    {
    public:

        /**
         * Creates a new PE4312 driver instance.
         */
        PE4312();

        /**
         * Returns a human-readable device name.
         *
         * @return Device name.
         */
        [[nodiscard]]
        std::string GetName() const override;

    protected:

        /**
         * Encodes an attenuation value into the 6-bit attenuation code
         * expected by the PE4312.
         *
         * The returned value represents only the attenuation field.
         * The base implementation of BuildControlWord() uses this value
         * directly as the control word.
         *
         * @param attenuationDb Attenuation in dB.
         *
         * @return Encoded attenuation code.
         */
        [[nodiscard]]
        uint16_t EncodeAttenuation(double attenuationDb) const override;

        /**
         * Programs the specified control word into the PE4312.
         *
         * Derived platform-specific implementations are responsible for
         * performing the actual SPI transaction required by the target
         * hardware.
         *
         * @param controlWord PE4312 control word.
         *
         * @return Operation status.
         */
        Error ProgramControlWord(uint16_t controlWord) override = 0;
    };
}

#endif //CPPFIRMWAREEMBEDDED_PE4312_HPP
