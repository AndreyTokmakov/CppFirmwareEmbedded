/**============================================================================
Name        : PE4312.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : PE4312.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_PE4312_HPP
#define CPPFIRMWAREEMBEDDED_PE4312_HPP

#include <cstdint>

#include "AttenuatorBase.hpp"

namespace rf
{   /**
     * @brief Driver for the pSemi PE4312 digital step attenuator.
     *
     * Features:
     *   - 0.0 ... 31.5 dB
     *   - 0.5 dB step
     *   - 6-bit attenuation word
     *
     * The class stores the requested attenuation locally.
     * Hardware is updated only after Apply() is called.
     */
    class PE4312 : public AttenuatorBase
    {
    public:

        PE4312();

        /**
         * @brief Returns device name.
         */
        std::string GetName() const override;

        /**
         * @brief Returns true because attenuation is programmable.
         */
        bool IsProgrammable() const override;

        /**
         * @brief Stores a new attenuation value.
         *
         * The value is rounded to the nearest supported step.
         * Hardware is not accessed until Apply() is called.
         *
         * @param attenuationDb Requested attenuation.
         */
        Error SetAttenuation(double attenuationDb) override;

        /**
         * @brief Returns current attenuation.
         */
        double GetAttenuation() const override;

        /**
         * @brief Returns minimum supported attenuation.
         */
        double GetMinimumAttenuation() const override;

        /**
         * @brief Returns maximum supported attenuation.
         */
        double GetMaximumAttenuation() const override;

        /**
         * @brief Returns attenuation resolution.
         */
        double GetStepSize() const override;

        /**
         * @brief Writes pending configuration to hardware.
         */
        Error Apply() override;

        /**
         * @brief Returns true while hardware update is in progress.
         */
        bool IsBusy() const override;

        /**
         * @brief Restores default attenuation (0 dB).
         */
        Error Reset() override;

    protected:

        /**
         * @brief Writes attenuation register to the hardware.
         *
         * Derived platform-specific classes should implement the
         * actual SPI transaction.
         *
         * @param value 6-bit attenuation register.
         */
        virtual Error WriteRegister(uint8_t value) = 0;

    private:

        /**
         * @brief Converts attenuation in dB to PE4312 register value.
         */
        static uint8_t EncodeAttenuation(double attenuationDb) ;

    private:

        double currentAttenuation = 0.0;
        double pendingAttenuation = 0.0;

        bool dirty = false;
        bool busy = false;
    };

}

#endif //CPPFIRMWAREEMBEDDED_PE4312_HPP
