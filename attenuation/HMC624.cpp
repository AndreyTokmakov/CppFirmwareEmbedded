/**============================================================================
Name        : HMC624.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : HMC624.cpp
============================================================================**/

#include "HMC624.hpp"

/******************************************************************************
 *
 *  HMC624.cpp
 *
 *  Implementation of the Analog Devices HMC624 digital step attenuator.
 *
 *  Overview
 *  --------
 *
 *  The HMC624 is a 6-bit parallel controlled RF digital step attenuator.
 *
 *  The device attenuation is selected by applying a binary control word to
 *  six digital input pins.
 *
 *  This class implements only the device-specific behavior:
 *
 *      • attenuation range
 *      • attenuation resolution
 *      • attenuation encoding
 *      • GPIO bit mapping
 *
 *  The generic GPIO update sequence is implemented by ParallelAttenuator.
 *
 ******************************************************************************/

namespace attenuation
{
    HMC624::HMC624(hal::IGpio& gpio): ParallelAttenuator(gpio){
    }

    std::string HMC624::getName() const {
        return "HMC624";
    }

    double HMC624::getMinimumAttenuation() const {
        return MinimumAttenuationDb;
    }

    double HMC624::getMaximumAttenuation() const {
        return MaximumAttenuationDb;
    }

    double HMC624::getStepSize() const {
        return StepSizeDb;
    }

    uint16_t HMC624::encodeAttenuation(const double attenuationDb) const{
        return static_cast<uint16_t>(attenuationDb / StepSizeDb);
    }

    uint8_t HMC624::getBitCount() const {
        return BitCount;
    }

    Error HMC624::writeBit(const uint8_t index, bool state){
        return gpio.write(index, state);
    }
}