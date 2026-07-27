/**============================================================================
Name        : IAttenuator.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : IAttenuator.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_IATTENUATOR_HPP
#define CPPFIRMWAREEMBEDDED_IATTENUATOR_HPP

#include <string>

#include "Error.hpp"

namespace attenuation
{
    using common::Error;

    /**
     * Common interface for RF attenuators.
     *
     * Overview
     * --------
     *
     * An RF attenuator reduces the amplitude of an RF signal by a specified amount
     * without intentionally introducing distortion. Attenuators are widely used in
     * RF receivers, transmitters, measurement equipment and automated test systems
     * to control signal levels, protect sensitive circuits and improve measurement
     * repeatability.
     *
     * This interface provides a hardware-independent abstraction for attenuation
     * control. Higher software layers interact only with this interface and remain
     * independent of the underlying hardware implementation.
     *
     * The interface supports both fixed and programmable attenuators.
     *
     * Fixed attenuators provide a constant attenuation determined entirely by the
     * hardware. Their attenuation cannot be modified after construction.
     *
     * Programmable attenuators allow the attenuation level to be changed through
     * a hardware interface such as SPI, I²C, GPIO or an integrated RF transceiver
     * register set.
     *
     * Scope
     * -----
     *
     * This interface is intentionally limited to attenuation control only.
     *
     * Operations such as enabling or disabling an RF signal path, bypassing the
     * attenuator, resetting hardware, waiting for asynchronous completion or
     * applying deferred configuration are not common to all attenuator devices.
     * Such functionality should be exposed through dedicated interfaces when
     * required by a particular hardware platform.
     *
     * Typical implementations
     * -----------------------
     *
     *     FixedAttenuator
     *     DigitalStepAttenuator
     *         ├── PE4312
     *         ├── PE43711
     *         └── HMC624
     *
     *     AD9361RxAttenuator
     *
     * Thread Safety
     * -------------
     *
     * Implementations are not required to be thread-safe. If an attenuator is
     * accessed concurrently from multiple threads, synchronization must be
     * provided by the caller.
     */
    struct IAttenuator
    {
        virtual ~IAttenuator() = default;

        /**
         * Returns a human-readable device name.
         *
         * The returned string should uniquely identify the attenuator model and
         * may be used for logging, diagnostics and user interfaces.
         *
         * @return Device name.
         */
        [[nodiscard]]
        virtual std::string getName() const = 0;

        /**
         * Returns whether the attenuation level can be modified.
         *
         * Fixed attenuators always return false. Programmable attenuators return
         * true.
         *
         * @return True if attenuation can be changed.
         */
        [[nodiscard]]
        virtual bool isProgrammable() const = 0;

        /**
         * Requests a new attenuation level.
         *
         * Programmable attenuators configure the hardware to achieve the requested
         * attenuation. Fixed attenuators typically return Error::NotSupported.
         *
         * The requested value may be rounded to the nearest supported attenuation
         * step depending on the hardware capabilities.
         *
         * @param attenuationDb Desired attenuation in decibels.
         *
         * @return Operation status.
         */
        virtual Error setAttenuation(double attenuationDb) = 0;

        /**
         * Returns the currently configured attenuation.
         *
         * For fixed attenuators this is the constant hardware attenuation.
         * For programmable attenuators this is the attenuation currently configured
         * by the device.
         *
         * @return Attenuation in decibels.
         */
        [[nodiscard]]
        virtual double getAttenuation() const = 0;

        /**
         * Returns the minimum attenuation supported by the hardware.
         *
         * @return Minimum attenuation in decibels.
         */
        [[nodiscard]]
        virtual double getMinimumAttenuation() const = 0;

        /**
         * Returns the maximum attenuation supported by the hardware.
         *
         * @return Maximum attenuation in decibels.
         */
        [[nodiscard]]
        virtual double getMaximumAttenuation() const = 0;

        /**
         * Returns the attenuation resolution.
         *
         * Programmable attenuators usually return their hardware step size
         * (for example 0.25 dB, 0.5 dB or 1.0 dB). Fixed attenuators return 0.0.
         *
         * @return Minimum programmable attenuation increment in decibels.
         */
        [[nodiscard]]
        virtual double getStepSize() const = 0;

        /**
         * Determines whether the specified attenuation can be represented by the
         * hardware.
         *
         * The method performs validation only and must not modify the hardware
         * state.
         *
         * @param attenuationDb Requested attenuation in decibels.
         *
         * @return True if the attenuation is supported.
         */
        [[nodiscard]]
        virtual bool isValidAttenuation(double attenuationDb) const = 0;

        /**
         * Returns the closest attenuation supported by the hardware.
         *
         * Values outside the supported range are clamped to the nearest valid
         * limit. Values between hardware steps are rounded according to the
         * device resolution.
         *
         * The hardware state is not modified.
         *
         * @param attenuationDb Requested attenuation in decibels.
         *
         * @return Closest representable attenuation.
         */
        [[nodiscard]]
        virtual double quantizeAttenuation(double attenuationDb) const = 0;
    };
    }

#endif //CPPFIRMWAREEMBEDDED_IATTENUATOR_HPP
