/**============================================================================
Name        : FixedAttenuator.cpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : FixedAttenuator.cpp
============================================================================**/

#include "FixedAttenuator.hpp"

/******************************************************************************
 *
 *  FixedAttenuator.cpp
 *
 *  Implementation of a fixed RF attenuator.
 *
 *  Overview
 *  --------
 *
 *  A fixed attenuator represents a passive RF component with a constant
 *  attenuation value. Unlike programmable attenuators, the attenuation is
 *  determined entirely by the hardware and cannot be modified during runtime.
 *
 *  Since the device contains no programmable logic, communication interface or
 *  internal state, this implementation simply exposes the hardware properties
 *  through the generic IAttenuator interface.
 *
 *  Calling setAttenuation() is not supported because the attenuation is fixed
 *  at manufacturing time.
 *
 ******************************************************************************/

namespace attenuation
{
    FixedAttenuator::FixedAttenuator(const double attenuationDb): attenuation(attenuationDb){
    }

    std::string FixedAttenuator::getName() const{
        return "Fixed Attenuator";
    }

    bool FixedAttenuator::isProgrammable() const {
        return false;
    }

    Error FixedAttenuator::setAttenuation(double) {
        return Error::NotSupported;
    }

    double FixedAttenuator::getAttenuation() const {
        return attenuation;
    }

    double FixedAttenuator::getMinimumAttenuation() const {
        return attenuation;
    }

    double FixedAttenuator::getMaximumAttenuation() const {
        return attenuation;
    }

    double FixedAttenuator::getStepSize() const {
        return 0.0;
    }
}