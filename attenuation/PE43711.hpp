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

#include "DigitalStepAttenuator.hpp"

namespace attenuation
{
    /**
     * Represents the pSemi PE43711 digital step attenuator.
     *
     * The PE43711 is a digitally controlled RF step attenuator providing
     * high-resolution attenuation control over a wide frequency range.
     * The device is programmed through a serial interface using a 7-bit
     * attenuation control code.
     *
     * Device characteristics:
     *   - Attenuation range: 0.0 dB to 31.75 dB
     *   - Resolution: 0.25 dB
     *   - 7-bit attenuation code
     *   - Low insertion loss
     *   - High attenuation accuracy
     *   - Broadband RF operation
     *
     * Typical applications:
     *   - Software Defined Radio (SDR)
     *   - RF front-end gain control
     *   - Automatic Gain Control (AGC)
     *   - Wireless communication systems
     *   - Test and measurement equipment
     *   - Radar systems
     *
     * This class implements only the PE43711-specific functionality.
     * All common programmable attenuator logic, including attenuation
     * validation, quantization, deferred configuration updates and state
     * management, is implemented by DigitalStepAttenuator.
     */
    class PE43711 : public DigitalStepAttenuator
    {
    public:

        /**
         * Creates a new PE43711 driver instance.
         */
        PE43711();

        /**
         * Returns a human-readable device name.
         *
         * @return Device name.
         */
        [[nodiscard]]
        std::string GetName() const override;

    protected:

        /**
         * Encodes an attenuation value into the 7-bit attenuation code
         * expected by the PE43711.
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
         * Programs the specified control word into the PE43711.
         *
         * Derived platform-specific implementations are responsible for
         * performing the actual SPI transaction required by the target
         * hardware.
         *
         * @param controlWord PE43711 control word.
         *
         * @return Operation status.
         */
        Error ProgramControlWord(uint16_t controlWord) override = 0;
    };
}
#endif //CPPFIRMWAREEMBEDDED_PE43711_HPP
