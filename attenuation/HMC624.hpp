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

#include "ParallelAttenuator.hpp"

/**
 * Driver model for the Analog Devices HMC624 digital step attenuator.
 *
 * Overview
 * --------
 *
 * The HMC624 is a broadband RF digital step attenuator controlled through a
 * parallel 6-bit digital interface.
 *
 * The device provides programmable attenuation by applying a binary control word
 * to six digital input pins.
 *
 * Typical applications:
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
 *         6-bit parallel GPIO interface
 *
 *     Number of control bits:
 *         6
 *
 * Architecture
 * ------------
 *
 * HMC624 contains only the device-specific logic.
 *
 * The common parallel GPIO handling is implemented by ParallelAttenuator:
 *
 *     HMC624
 *        |
 *        v
 *     ParallelAttenuator
 *        |
 *        v
 *     IGpio
 *
 * HMC624 is responsible for:
 *     • attenuation limits
 *     • attenuation step size
 *     • attenuation encoding
 *     • GPIO bit mapping
 *
 * ParallelAttenuator is responsible for:
 *     • writing GPIO states
 *     • updating the parallel register
 *
 * Control word encoding
 * ---------------------
 *
 * The HMC624 uses a 6-bit binary control word:
 *
 *     Bit0  - 0.5 dB
 *     Bit1  - 1.0 dB
 *     Bit2  - 2.0 dB
 *     Bit3  - 4.0 dB
 *     Bit4  - 8.0 dB
 *     Bit5  - 16.0 dB
 *
 * Examples:
 *     0.0 dB:
 *         000000
 *     0.5 dB:
 *         000001
 *     10.0 dB:
 *         010101
 *     31.5 dB:
 *         111111
 */

namespace attenuation
{
    class HMC624 : public ParallelAttenuator
    {
    public:

        /**
         * Creates an HMC624 attenuator instance.
         *
         * @param gpio GPIO controller used for digital control lines.
         */
        explicit HMC624(hal::IGpio& gpio);


        /**
         * Returns device name.
         *
         * @return "HMC624".
         */
        [[nodiscard]]
        std::string getName() const override;

        /**
         * Returns minimum supported attenuation.
         *
         * @return 0.0 dB.
         */
        [[nodiscard]]
        double getMinimumAttenuation() const override;

        /**
         * Returns maximum supported attenuation.
         *
         * @return 31.5 dB.
         */
        [[nodiscard]]
        double getMaximumAttenuation() const override;

        /**
         * Returns attenuation resolution.
         *
         * @return 0.5 dB.
         */
        [[nodiscard]]
        double getStepSize() const override;

    protected:

        /**
         * Converts attenuation value into HMC624 control word.
         *
         * Encoding:
         *     register = attenuation / 0.5
         *
         * Examples:
         *     0.0 dB  -> 0b000000
         *     0.5 dB  -> 0b000001
         *     10.0 dB -> 0b010100
         *     31.5 dB -> 0b111111
         *
         * @param attenuationDb Quantized attenuation value.
         *
         * @return Six-bit control word.
         */
        [[nodiscard]]
        uint16_t encodeAttenuation(double attenuationDb) const override;

        /**
         * Returns number of GPIO control bits.
         *
         * @return 6.
         */
        [[nodiscard]]
        uint8_t getBitCount() const override;

        /**
         * Writes one attenuation control bit.
         * Maps logical attenuation bits to physical GPIO pins.
         *
         * The default implementation assumes:
         *     bit0 -> GPIO 0
         *     bit1 -> GPIO 1
         *     ...
         *     bit5 -> GPIO 5
         *
         * Board-specific implementations may override this method.
         *
         * @param index Bit index.
         * @param state Desired logic state.
         *
         * @return Operation status.
         */
        Error writeBit(uint8_t index, bool state) override;


    private:

        static constexpr uint8_t BitCount = 6;
        static constexpr double MinimumAttenuationDb = 0.0;
        static constexpr double MaximumAttenuationDb = 31.5;
        static constexpr double StepSizeDb = 0.5;
    };

} // namespace attenuation

#endif //CPPFIRMWAREEMBEDDED_HMC624_HPP
