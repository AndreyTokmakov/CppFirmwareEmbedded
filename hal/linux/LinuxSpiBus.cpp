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

namespace hal
{
    LinuxSpiBus::LinuxSpiBus(
        const std::string& device,
        uint32_t frequency,
        uint8_t spiMode,
        uint8_t wordSize)
        :
        devicePath(device),
        frequencyHz(frequency),
        mode(spiMode),
        bitsPerWord(wordSize)
    {
        if (open() != Error::None)
        {
            return;
        }


        configure();
    }


    LinuxSpiBus::~LinuxSpiBus()
    {
        if (fileDescriptor >= 0)
        {
            close(fileDescriptor);

            fileDescriptor = -1;
        }
    }


    Error LinuxSpiBus::open()
    {
        fileDescriptor =
            ::open(
                devicePath.c_str(),
                O_RDWR);


        if (fileDescriptor < 0)
        {
            return Error::HardwareFailure;
        }


        return Error::None;
    }


    Error LinuxSpiBus::configure()
    {
        if (fileDescriptor < 0)
        {
            return Error::HardwareFailure;
        }


        Error result =
            setMode(mode);


        if (result != Error::None)
        {
            return result;
        }


        result =
            setBitsPerWord(bitsPerWord);


        if (result != Error::None)
        {
            return result;
        }


        return setFrequency(frequencyHz);
    }


    Error LinuxSpiBus::transfer(
        const uint8_t* txData,
        uint8_t* rxData,
        uint32_t size)
    {
        if (fileDescriptor < 0)
        {
            return Error::HardwareFailure;
        }


        spi_ioc_transfer transfer{};


        transfer.tx_buf =
            reinterpret_cast<uint64_t>(txData);


        transfer.rx_buf =
            reinterpret_cast<uint64_t>(rxData);


        transfer.len =
            size;


        transfer.speed_hz =
            frequencyHz;


        transfer.bits_per_word =
            bitsPerWord;


        const int result =
            ioctl(
                fileDescriptor,
                SPI_IOC_MESSAGE(1),
                &transfer);


        if (result < 0)
        {
            return Error::HardwareFailure;
        }


        return Error::None;
    }


    Error LinuxSpiBus::setFrequency(
        uint32_t frequency)
    {
        if (fileDescriptor < 0)
        {
            return Error::HardwareFailure;
        }


        uint32_t value = frequency;


        const int result =
            ioctl(
                fileDescriptor,
                SPI_IOC_WR_MAX_SPEED_HZ,
                &value);


        if (result < 0)
        {
            return Error::HardwareFailure;
        }


        frequencyHz = frequency;


        return Error::None;
    }


    uint32_t LinuxSpiBus::getFrequency() const
    {
        return frequencyHz;
    }


    Error LinuxSpiBus::setMode(
        uint8_t spiMode)
    {
        if (fileDescriptor < 0)
        {
            return Error::HardwareFailure;
        }


        const int result =
            ioctl(
                fileDescriptor,
                SPI_IOC_WR_MODE,
                &spiMode);


        if (result < 0)
        {
            return Error::HardwareFailure;
        }


        mode = spiMode;


        return Error::None;
    }


    uint8_t LinuxSpiBus::getMode() const
    {
        return mode;
    }


    Error LinuxSpiBus::setBitsPerWord(
        uint8_t value)
    {
        if (fileDescriptor < 0)
        {
            return Error::HardwareFailure;
        }


        const int result =
            ioctl(
                fileDescriptor,
                SPI_IOC_WR_BITS_PER_WORD,
                &value);


        if (result < 0)
        {
            return Error::HardwareFailure;
        }


        bitsPerWord = value;


        return Error::None;
    }


    uint8_t LinuxSpiBus::getBitsPerWord() const
    {
        return bitsPerWord;
    }


    bool LinuxSpiBus::isOpen() const
    {
        return fileDescriptor >= 0;
    }


} // namespace hal