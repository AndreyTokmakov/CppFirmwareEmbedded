/**============================================================================
Name        : LinuxSpiBus.cpp
Created on  : 25.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxSpiBus.cpp
============================================================================**/

#include "LinuxSpiBus.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

#include <utility>

namespace hal
{
    LinuxSpiBus::LinuxSpiBus(std::string  device,
                             const uint32_t frequencyHz,
                             const uint8_t mode,
                             const uint8_t bitsPerWord):
        devicePath(std::move(device)),
        frequencyHz(frequencyHz),
        mode(mode),
        bitsPerWord(bitsPerWord)
    {
        if (open() != Error::Success){
            return;
        }

        if (Error::Success == configure()) {
            throw std::runtime_error("Error in LinuxSpiBus::configure()");
        }
    }


    LinuxSpiBus::~LinuxSpiBus()
    {
        if (fileDescriptor >= 0)
        {
            ::close(fileDescriptor);
            fileDescriptor = -1;
        }
    }

    Error LinuxSpiBus::open()
    {
        fileDescriptor = ::open(devicePath.c_str(),O_RDWR);
        if (fileDescriptor < 0) {
            return Error::HardwareFailure;
        }
        return Error::Success;
    }

    Error LinuxSpiBus::configure()
    {
        if (fileDescriptor < 0) {
            return Error::HardwareFailure;
        }

        Error result = setMode(mode);
        if (result != Error::Success) {
            return result;
        }

        result = setBitsPerWord(bitsPerWord);
        if (result != Error::Success) {
            return result;
        }

        return setFrequency(frequencyHz);
    }


    Error LinuxSpiBus::transfer(const uint8_t* txData,
                                uint8_t* rxData,
                                const uint32_t size)
    {
        if (fileDescriptor < 0) {
            return Error::HardwareFailure;
        }

        spi_ioc_transfer transfer{};
        transfer.tx_buf = reinterpret_cast<uint64_t>(txData);
        transfer.rx_buf = reinterpret_cast<uint64_t>(rxData);
        transfer.len = size;
        transfer.speed_hz = frequencyHz;
        transfer.bits_per_word = bitsPerWord;

        if (const int result = ::ioctl(fileDescriptor, SPI_IOC_MESSAGE(1), &transfer); result < 0) {
            return Error::HardwareFailure;
        }

        return Error::Success;
    }

    Error LinuxSpiBus::setFrequency(const uint32_t frequency)
    {
        if (fileDescriptor < 0) {
            return Error::HardwareFailure;
        }

        uint32_t value = frequency;
        if (const int result = ::ioctl(fileDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &value); result < 0) {
            return Error::HardwareFailure;
        }

        frequencyHz = frequency;
        return Error::Success;
    }

    uint32_t LinuxSpiBus::getFrequency() const noexcept {
        return frequencyHz;
    }

    Error LinuxSpiBus::setMode(uint8_t spiMode)
    {
        if (fileDescriptor < 0) {
            return Error::HardwareFailure;
        }

        if (const int result = ::ioctl(fileDescriptor, SPI_IOC_WR_MODE, &spiMode); result < 0) {
            return Error::HardwareFailure;
        }

        mode = spiMode;
        return Error::Success;
    }

    uint8_t LinuxSpiBus::getMode() const noexcept {
        return mode;
    }

    Error LinuxSpiBus::setBitsPerWord(const uint8_t bits)
    {
        if (fileDescriptor < 0){
            return Error::HardwareFailure;
        }

        if (const int result = ::ioctl(fileDescriptor, SPI_IOC_WR_BITS_PER_WORD, &bits); result < 0){
            return Error::HardwareFailure;
        }

        bitsPerWord = bits;
        return Error::Success;
    }


    uint8_t LinuxSpiBus::getBitsPerWord() const noexcept {
        return bitsPerWord;
    }

    bool LinuxSpiBus::isOpen() const noexcept{
        return fileDescriptor >= 0;
    }

} // namespace hal