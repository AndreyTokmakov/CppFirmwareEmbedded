/**============================================================================
Name        : DigitalStepAttenuator.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : DigitalStepAttenuator.cpp
============================================================================**/

#include "DigitalStepAttenuator.hpp"

/******************************************************************************
     *
     *  DigitalStepAttenuator.cpp
     *
     *  Implementation of the common functionality shared by programmable RF
     *  digital step attenuators.
     *
     *  Overview
     *  --------
     *
     *  Most programmable RF attenuators operate in nearly the same way regardless
     *  of the manufacturer or communication interface.
     *
     *  An application requests an attenuation value expressed in decibels.
     *  The driver verifies that the value is within the supported range, rounds
     *  it to the nearest hardware step, converts it into a device-specific
     *  register representation and finally writes the encoded value to the
     *  hardware.
     *
     *  The register layout differs between attenuator families, but the overall
     *  sequence of operations remains essentially identical.
     *
     *  This file implements that common algorithm once using the Template Method
     *  design pattern. Device-specific drivers only provide:
     *
     *      • attenuation register encoding
     *      • hardware register write
     *
     *  This significantly reduces duplicated code between drivers such as:
     *
     *      • PE4312
     *      • PE43711
     *      • HMC624
     *
     *  or other compatible digital step attenuators.
     *
     *  Attenuation Update Sequence
     *  ---------------------------
     *
     *      Requested attenuation
     *              │
     *              ▼
     *      Validate range
     *              │
     *              ▼
     *      Quantize to nearest hardware step
     *              │
     *              ▼
     *      Encode hardware register
     *              │
     *              ▼
     *      Write register
     *              │
     *              ▼
     *      Store current attenuation
     *              │
     *              ▼
     *      Optional notification hook
     *
******************************************************************************/

namespace attenuation
{
    bool DigitalStepAttenuator::isProgrammable() const{
        return true;
    }

    Error DigitalStepAttenuator::setAttenuation(double attenuationDb)
    {
        if (!isValidAttenuation(attenuationDb)){
            return Error::OutOfRange;
        }

        attenuationDb = quantizeAttenuation(attenuationDb);
        const uint16_t registerValue = encodeAttenuation(attenuationDb);
        const Error result = writeAttenuationRegister(registerValue);
        if (result != Error::Success){
            return result;
        }

        attenuation = attenuationDb;
        onAttenuationChanged(attenuationDb);
        return Error::Success;
    }

    double DigitalStepAttenuator::getAttenuation() const{
        return attenuation;
    }

    void DigitalStepAttenuator::onAttenuationChanged(double) {
        // Default implementation intentionally does nothing.
    }

}