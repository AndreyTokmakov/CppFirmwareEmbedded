/**============================================================================
Name        : ADRF5700.hpp
Created on  : 27.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : ADRF5700.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_ADRF5700_HPP
#define CPPFIRMWAREEMBEDDED_ADRF5700_HPP

#include "SpiAttenuator.hpp"

/**
 * Driver model for the Analog Devices ADRF5700 digital step attenuator.
 *
 * Overview
 * --------
 *
 * The ADRF5700 is a high-performance RF digital step attenuator with an
 * SPI-compatible serial control interface.
 *
 * It is designed for broadband RF applications requiring accurate gain
 * control.
 *
 * Typical applications:
 *
 *     • RF transceivers
 *     • Test equipment
 *     • Microwave systems
 *     • Automatic gain control
 *
 *
 * Architecture
 * ------------
 *
 * ADRF5700 contains only device-specific logic.
 *
 *
 *     ADRF5700
 *          |
 *          v
 *     SpiAttenuator
 *          |
 *          v
 *       ISpiBus
 *
 *
 * Responsibilities:
 *
 *     ADRF5700:
 *
 *         • attenuation characteristics
 *         • register encoding
 *         • SPI frame generation
 *
 *
 *     SpiAttenuator:
 *
 *         • SPI communication
 *         • bus access
 *
 */

namespace attenuation
{
    class ADRF5700 : public SpiAttenuator
    {
    public:

        explicit ADRF5700(hal::ISpiBus& spi);

        [[nodiscard]]
        std::string getName() const override;

        [[nodiscard]]
        double getMinimumAttenuation() const override;

        [[nodiscard]]
        double getMaximumAttenuation() const override;

        [[nodiscard]]
        double getStepSize() const override;

    protected:

        /**
         * Converts attenuation value into ADRF5700 register code.
         */
        [[nodiscard]]
        uint16_t encodeAttenuation(double attenuationDb) const override;

        /**
         * Builds ADRF5700 SPI frame.
         */
        [[nodiscard]]
        uint16_t buildSpiWord(uint16_t value) const override;


    private:

        static constexpr double MinimumAttenuationDb = 0.0;
        static constexpr double MaximumAttenuationDb = 31.5;
        static constexpr double StepSizeDb = 0.5;
    };
}
#endif //CPPFIRMWAREEMBEDDED_ADRF5700_HPP
