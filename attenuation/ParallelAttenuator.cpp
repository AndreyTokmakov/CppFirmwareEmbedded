/**============================================================================
Name        : ParallelAttenuator.cpp
Created on  : 27.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : ParallelAttenuator.cpp
============================================================================**/

#include "ParallelAttenuator.hpp"

/******************************************************************************
 *
 *  ParallelAttenuator.cpp
 *
 *  Common implementation for GPIO-controlled RF digital step attenuators.
 *
 *  Overview
 *  --------
 *
 *  Some RF digital attenuators are controlled through parallel digital inputs
 *  instead of a serial interface such as SPI.
 *
 *  The attenuation update sequence is:
 *
 *      Requested attenuation
 *              │
 *              ▼
 *      Encode attenuation register
 *              │
 *              ▼
 *      Write register bits to GPIO outputs
 *              │
 *              ▼
 *      Apply latch pulse (if required)
 *
 *  This class implements the common GPIO update mechanism. Device-specific
 *  drivers are responsible only for:
 *
 *      • attenuation encoding
 *      • GPIO bit mapping
 *      • number of control bits
 *
 *  Typical derived devices:
 *
 *      • HMC624
 *      • HMC540
 *
 *  The class does not know about the physical GPIO controller or platform.
 *  Hardware access is delegated through IGpio and device-specific mapping.
 *
 ******************************************************************************/

#include "ParallelAttenuator.hpp"

namespace attenuation
{
    ParallelAttenuator::ParallelAttenuator(hal::IGpio& gpioController) : gpio(gpioController)
    {
    }

    Error ParallelAttenuator::writeAttenuationRegister(const uint16_t value)
    {
        const uint8_t bitCount = getBitCount();
        for (uint8_t index = 0; index < bitCount; ++index)
        {
            const bool state = ((value >> index) & 0x01U) != 0;
            const Error result = writeBit(index, state);
            if (result != Error::Success)
            {
                return result;
            }
        }

        return applyParallelValue();
    }

    Error ParallelAttenuator::applyParallelValue()
    {
        return Error::Success;
    }

}