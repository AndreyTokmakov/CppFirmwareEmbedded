/**============================================================================
Name        : ISpiBus.hpp
Created on  : 27.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : ISpiBus.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_ISPIBUS_HPP
#define CPPFIRMWAREEMBEDDED_ISPIBUS_HPP

#include <cstdint>
#include <iostream>
#include "Error.hpp"

namespace hal
{
    using common::Error;

    /**
     * SPI clock polarity and phase configuration.
     *
     * Defines how data is sampled and shifted on the SPI bus.
     */
    enum class SpiMode : uint8_t
    {
        Mode0,
        Mode1,
        Mode2,
        Mode3
    };


    /**
     * Abstract SPI bus interface.
     *
     * Overview
     * --------
     *
     * Provides hardware-independent access to an SPI controller.
     *
     * Typical implementations:
     *
     *     • Linux spidev
     *     • STM32 SPI peripheral
     *     • FPGA SPI controller
     *     • Mock SPI bus for unit tests
     *
     * Responsibilities:
     *
     *     • SPI configuration
     *     • data transmission
     *
     * This interface intentionally does not know about specific devices.
     * Device drivers such as:
     *
     *     • RF attenuators
     *     • PLL synthesizers
     *     • ADC/DAC converters
     *
     * operate through this abstraction.
     */
    class ISpiBus
    {
    public:

        virtual ~ISpiBus() = default;


        /**
         * Returns current SPI mode.
         */
        [[nodiscard]]
        virtual SpiMode getMode() const = 0;


        /**
         * Changes SPI mode.
         *
         * @param mode New SPI mode.
         */
        virtual Error setMode(SpiMode mode) = 0;


        /**
         * Returns current SPI clock frequency.
         *
         * @return Frequency in Hz.
         */
        [[nodiscard]]
        virtual uint32_t getFrequency() const = 0;


        /**
         * Changes SPI clock frequency.
         *
         * @param frequencyHz New frequency.
         */
        virtual Error setFrequency(uint32_t frequencyHz) = 0;


        /**
         * Writes a single 16-bit word to the SPI bus.
         *
         * This helper exists because many RF ICs use fixed-size registers.
         *
         * @param value Data word.
         *
         * @return Operation status.
         */
        virtual Error write(uint16_t value) = 0;


        /**
         * Writes raw byte buffer.
         *
         * Default implementation is optional and may be overridden by hardware
         * specific implementations.
         *
         * @param data Buffer.
         * @param size Buffer size.
         *
         * @return Operation status.
         */
        virtual Error write(const uint8_t* data, uint32_t size) = 0;
    };


    /**
     * Simple header-only mock SPI bus.
     *
     * Used for:
     *
     *     • unit tests
     *     • driver bring-up
     *     • simulation
     *
     * Does not access real hardware.
     */
    class MockSpiBus final : public ISpiBus
    {
    public:

        SpiMode getMode() const override {
            return mode;
        }

        Error setMode(const SpiMode newMode) override {
            mode = newMode;
            return Error::Success;
        }

        uint32_t getFrequency() const override {
            return frequencyHz;
        }

        Error setFrequency(const uint32_t newFrequencyHz) override
        {
            frequencyHz = newFrequencyHz;
            return Error::Success;
        }

        Error write(const uint16_t value) override
        {
            lastWord = value;
            std::cout << "SPI write16: 0x" << std::hex << value << std::dec<< '\n';
            return Error::Success;
        }

        Error write(const uint8_t* data, uint32_t size) override
        {
            if (data == nullptr || size == 0){
                return Error::InvalidParameter;
            }

            lastByteSize = size;
            return Error::Success;
        }

        [[nodiscard]]
        uint16_t getLastWord() const {
            return lastWord;
        }

    private:

        SpiMode mode { SpiMode::Mode0 };
        uint32_t frequencyHz { 1'000'000 };
        uint16_t lastWord { 0 };
        uint32_t lastByteSize { 0 };
    };


} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_ISPIBUS_HPP
