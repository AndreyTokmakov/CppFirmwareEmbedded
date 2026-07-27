/**============================================================================
Name        : DigitalStepAttenuator.hpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : DigitalStepAttenuator.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_DIGITALSTEPATTENUATOR_HPP
#define CPPFIRMWAREEMBEDDED_DIGITALSTEPATTENUATOR_HPP

#include <cstdint>

#include "AttenuatorBase.hpp"

namespace attenuation
{
    /**
     * Base class for programmable digital step attenuators.
     *
     * Overview
     * --------
     *
     * Digital step attenuators provide programmable RF attenuation by selecting one
     * of many discrete attenuation levels. Unlike fixed attenuators, these devices
     * contain digital control logic that configures an internal resistor network
     * through a serial or parallel control interface.
     *
     * This class implements functionality common to most programmable attenuator
     * drivers using the Template Method design pattern. The complete attenuation
     * update algorithm is implemented once in this class, while derived classes
     * provide only the hardware-specific operations required to communicate with a
     * particular device.
     *
     * Typical devices include:
     *
     *     • pSemi PE4312
     *     • pSemi PE43711
     *     • Analog Devices HMC624
     *     • Similar SPI, GPIO or I²C controlled RF attenuators
     *
     * Responsibilities
     * ----------------
     *
     * This class provides:
     *     • storage of the current attenuation
     *     • attenuation validation
     *     • attenuation quantization
     *     • common implementation of setAttenuation()
     *     • reporting that the device is programmable
     *
     * Derived classes provide:
     *     • device identification
     *     • attenuation range
     *     • attenuation resolution
     *     • attenuation register encoding
     *     • hardware register write
     *
     * Hardware Model
     * --------------
     *
     * Nearly every programmable RF attenuator performs the following sequence:
     *
     *     Requested attenuation
     *             ↓
     *     Validate range
     *             ↓
     *     Quantize to hardware step
     *             ↓
     *     Encode hardware register
     *             ↓
     *     Write register
     *             ↓
     *     Store current attenuation
     *
     * Although different devices use different register layouts and communication
     * interfaces, the overall algorithm remains identical. This class implements
     * the common algorithm once, allowing derived classes to focus solely on the
     * device-specific behavior.
     *
     * Thread Safety
     * -------------
     *
     * Instances are not internally synchronized. Concurrent access from multiple
     * threads must be synchronized by the caller.
     */
    class DigitalStepAttenuator : public AttenuatorBase
    {
    public:

        /**
         * Creates a programmable attenuator.
         */
        DigitalStepAttenuator() = default;

        /**
         * Indicates that this attenuator is programmable.
         *
         * @return Always true.
         */
        [[nodiscard]]
        bool isProgrammable() const override;

        /**
         * Sets the attenuation.
         *
         * The requested attenuation is validated, quantized to the nearest
         * supported hardware step, converted into a device-specific register
         * representation and written to the hardware.
         *
         * The complete update algorithm is implemented by this class and is shared
         * by all programmable attenuator drivers.
         *
         * @param attenuationDb Desired attenuation in decibels.
         *
         * @return Operation status.
         */
        Error setAttenuation(double attenuationDb) final override;

        /**
         * Returns the currently configured attenuation.
         *
         * @return Current attenuation in decibels.
         */
        [[nodiscard]]
        double getAttenuation() const override;

    protected:

        /**
         * Converts an attenuation value into a device-specific register value.
         *
         * The supplied attenuation has already been validated and quantized to a
         * value supported by the hardware.
         *
         * Different attenuator families use different register layouts, therefore
         * every derived class implements its own conversion.
         *
         * @param attenuationDb Quantized attenuation in decibels.
         *
         * @return Encoded hardware register value.
         */
        [[nodiscard]]
        virtual uint16_t encodeAttenuation(double attenuationDb) const = 0;

        /**
         * Writes the encoded attenuation value to the hardware.
         *
         * The communication interface depends on the target platform and the
         * specific attenuator device. Typical implementations use SPI, GPIO or
         * another hardware transport.
         *
         * @param value Encoded attenuation register.
         *
         * @return Operation status.
         */
        virtual Error writeAttenuationRegister(uint16_t value) = 0;

        /**
         * Called after the attenuation has been successfully written to the
         * hardware.
         *
         * The default implementation does nothing. Derived classes may override
         * this method to update cached state, notify dependent components or
         * perform additional device-specific actions after a successful hardware
         * update.
         *
         * @param attenuationDb Applied attenuation in decibels.
         */
        virtual void onAttenuationChanged(double attenuationDb);

    private:

        double attenuation;
    };
}

#endif //CPPFIRMWAREEMBEDDED_DIGITALSTEPATTENUATOR_HPP
