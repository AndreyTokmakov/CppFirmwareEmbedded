/**============================================================================
Name        : ParallelAttenuator.hpp
Created on  : 27.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : ParallelAttenuator.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_PARALLELATTENUATOR_HPP
#define CPPFIRMWAREEMBEDDED_PARALLELATTENUATOR_HPP

#include <cstdint>

#include "DigitalStepAttenuator.hpp"
#include "IGpio.hpp"

/**
 * Base class for parallel-controlled RF digital step attenuators.
 *
 * Overview
 * --------
 *
 * Some RF digital attenuators do not use a serial interface and instead expose
 * their attenuation value through a set of parallel digital control inputs.
 *
 * The attenuation value is represented by a binary word connected directly to
 * device control pins.
 *
 * Typical devices:
 *
 *     • Analog Devices HMC624
 *     • HMC540
 *     • Similar GPIO-controlled RF attenuators
 *
 * Hardware model:
 *
 *     Attenuation value
 *             |
 *             v
 *     Encode register bits
 *             |
 *             v
 *     Set GPIO outputs
 *             |
 *             v
 *     Pulse latch enable
 *
 * Responsibilities
 * ----------------
 *
 * This class implements:
 *
 *     • GPIO based register update
 *     • binary word output
 *     • latch control sequence
 *
 * Derived classes implement:
 *
 *     • attenuation encoding
 *     • number of control bits
 *     • device-specific timing requirements
 *
 * Thread Safety
 * -------------
 *
 * This class is not internally synchronized.
 */

namespace attenuation
{
    class ParallelAttenuator : public DigitalStepAttenuator
    {
    public:

        /**
         * Creates a parallel controlled attenuator.
         *
         * @param gpio GPIO controller used for device pins.
         */
        explicit ParallelAttenuator(hal::IGpio& gpio);


    protected:

        /**
         * Writes encoded attenuation value to the hardware.
         *
         * The value is converted into GPIO states and optionally followed by a
         * latch-enable pulse.
         *
         * @param value Encoded attenuation register.
         * @return Operation status.
         */
        Error writeAttenuationRegister(uint16_t value) final;


        /**
         * Writes a single bit to the corresponding GPIO line.
         *
         * Derived classes provide the mapping between register bits and GPIO pins.
         *
         * @param index GPIO bit index.
         * @param state Desired output state.
         * @return Operation status.
         */
        virtual Error writeBit(uint8_t index, bool state) = 0;


        /**
         * Returns the number of active control bits.
         *
         * Example:
         *
         *     HMC624:
         *         6 bits
         *
         * @return Number of GPIO controlled bits.
         */
        [[nodiscard]]
        virtual uint8_t getBitCount() const = 0;


        /**
         * Applies the new parallel value.
         *
         * Some devices require a latch-enable pulse after updating the GPIO lines.
         * Devices without a latch can leave the default implementation unchanged.
         *
         * @return Operation status.
         */
        virtual Error applyParallelValue();


    protected:

        hal::IGpio& gpio;
    };

} // namespace attenuation

#endif //CPPFIRMWAREEMBEDDED_PARALLELATTENUATOR_HPP
