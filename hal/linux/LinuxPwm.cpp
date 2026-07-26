/**============================================================================
Name        : LinuxPwm.cpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxPwm.cpp
============================================================================**/

#include "LinuxPwm.hpp"
#include <fstream>
#include <iostream>


/******************************************************************************
 *
 *  LinuxPwm.cpp
 *
 *  Linux implementation of the HAL PWM interface.
 *
 *  Overview
 *  --------
 *
 *  This module implements the IPwm interface using the Linux PWM sysfs
 *  subsystem.
 *
 *  Unlike microcontroller firmware, user-space applications running under
 *  Linux do not access timer registers directly. Instead, all interaction
 *  with PWM hardware is performed through the Linux PWM framework.
 *
 *  The Linux kernel exposes each PWM controller as a directory under:
 *
 *      /sys/class/pwm/
 *
 *  Each PWM controller (pwmchip) provides one or more PWM output channels.
 *  Before a channel can be configured it must first be exported, after which
 *  Linux creates a directory containing several control files:
 *
 *      enable
 *      period
 *      duty_cycle
 *      polarity
 *
 *  Configuration is performed by writing ASCII values into these files.
 *  The Linux kernel validates all requested parameters and forwards them
 *  to the underlying hardware driver.
 *
 *
 *  Hardware Abstraction
 *  --------------------
 *
 *  LinuxPwm intentionally hides all Linux-specific details from higher
 *  software layers.
 *
 *  Firmware libraries should depend only on the IPwm interface and never
 *  directly manipulate files located in /sys/class/pwm.
 *
 *  This design allows the same firmware code to execute on:
 *
 *      • Embedded Linux
 *      • STM32
 *      • FPGA
 *      • RTOS
 *
 *  by simply replacing the IPwm implementation.
 *
 *
 *  PWM Representation
 *  ------------------
 *
 *  The HAL interface exposes:
 *
 *      Frequency  [Hz]
 *      Duty Cycle [0.0 ... 1.0]
 *
 *  while Linux internally expects:
 *
 *      Period      [ns]
 *      Duty Cycle  [ns]
 *
 *  LinuxPwm performs the required conversions automatically.
 *
 *
 *  Resource Management
 *  -------------------
 *
 *  The PWM channel is exported automatically during object construction
 *  if necessary.
 *
 *  Likewise, the channel is automatically unexported when the object is
 *  destroyed.
 *
 *  This implementation follows the RAII principle to ensure that Linux
 *  resources are released correctly.
 *
 *
 *  Thread Safety
 *  -------------
 *
 *  LinuxPwm instances are not internally synchronized.
 *
 *  If multiple threads access the same PWM channel simultaneously,
 *  synchronization must be provided by the caller.
 *
 *
 *  Dependencies
 *  ------------
 *
 *      Linux kernel PWM framework
 *
 *      CONFIG_PWM
 *      CONFIG_SYSFS
 *
 *      <filesystem>
 *      <fstream>
 *
 ******************************************************************************/

namespace
{
    using namespace std::string_view_literals;

    constexpr std::string_view kPwmDirectoryPrefix = "pwm"sv;
    constexpr std::string_view kExportFileName = "export"sv;
    constexpr std::string_view kUnexportFileName = "unexport"sv;
    constexpr std::string_view kEnableFileName = "enable"sv;
    constexpr std::string_view kPeriodFileName = "period"sv;
    constexpr std::string_view kDutyCycleFileName = "duty_cycle"sv;
    constexpr std::string_view kPolarityFileName = "polarity"sv;
    constexpr std::string_view kEnableValue = "1"sv;
    constexpr std::string_view kDisableValue = "0"sv;

    constexpr uint64_t kNanosecondsPerSecond = 1'000'000'000ULL;
}

namespace hal
{
    LinuxPwm::LinuxPwm(const std::filesystem::path& pwmChip,const uint32_t channel):
        pwmChipPath(pwmChip),
        pwmPath(pwmChip / (kPwmDirectoryPrefix + std::to_string(channel))),
        channel(channel)
    {
    }

    Error LinuxPwm::open()
    {
        if (opened){
            return Error::Success;
        }

        Error result = exportChannel();
        if (result != Error::Success) {
            return result;
        }

        result = setFrequency(frequency);
        if (result != Error::Success) {
            return result;
        }

        result = setDutyCycle(dutyCycle);
        if (result != Error::Success) {
            return result;
        }

        opened = true;

        return Error::Success;
    }

    Error LinuxPwm::close()
    {
        if (!opened){
            return Error::Success;
        }

        Error result = disable();
        if (result != Error::Success) {
            return result;
        }

        result = unexportChannel();
        if (result != Error::Success) {
            return result;
        }

        opened = false;
        return Error::Success;
    }

    Error LinuxPwm::enable()
    {
        if (!opened) {
            return Error::NotInitialized;
        }
        return writeFile(pwmPath / kEnableFileName, kEnableValue);
    }

    Error LinuxPwm::disable()
    {
        if (!opened){
            return Error::NotInitialized;
        }
        return writeFile(pwmPath / kEnableFileName, kDisableValue);
    }

    Error LinuxPwm::setFrequency(const uint32_t frequencyHz)
    {
        if (frequencyHz == 0) {
            return Error::InvalidParameter;
        }

        frequency = frequencyHz;
        if (!opened){
            return Error::Success;
        }

        const uint64_t period = frequencyToPeriod(frequency);
        const Error result = writeFile(pwmPath /  kPeriodFileName, std::to_string(period));
        if (result != Error::Success){
            return result;
        }

        const uint64_t duty = static_cast<uint64_t>(static_cast<double>(period) * dutyCycle);
        return writeFile(pwmPath /  kDutyCycleFileName, std::to_string(duty));
    }

    Error LinuxPwm::setDutyCycle(const double value)
    {
        if (value < 0.0 || value > 1.0){
            return Error::InvalidParameter;
        }

        dutyCycle = value;
        if (!opened) {
            return Error::Success;
        }

        const uint64_t period = frequencyToPeriod(frequency);
        const uint64_t duty = static_cast<uint64_t>(static_cast<double>(period) * dutyCycle);

        return writeFile(pwmPath / kDutyCycleFileName, std::to_string(duty));
    }

    uint32_t LinuxPwm::getFrequency() const noexcept {
        return frequency;
    }

    double LinuxPwm::getDutyCycle() const noexcept {
        return dutyCycle;
    }

    bool LinuxPwm::isOpen() const noexcept {
        return opened;
    }

    Error LinuxPwm::exportChannel()
    {
        if (std::filesystem::exists(pwmPath)){
            exported = false;
            return Error::Success;
        }

        Error result = writeFile(pwmChipPath / kExportFileName, std::to_string(channel));
        if (result == Error::Success){
            exported = true;
        }

        return result;
    }

    Error LinuxPwm::unexportChannel() const
    {
        if (!exported){
            return Error::Success;
        }

        return writeFile(pwmChipPath / kUnexportFileName, std::to_string(channel));
    }
    
    Error LinuxPwm::writeFile(const std::filesystem::path& path,
                              const std::string_view value)
    {
        std::ofstream file(path);
        if (!file) {
            return Error::HardwareFailure;
        }

        file << value;
        if (!file.good()){
            return Error::HardwareFailure;
        }

        return Error::Success;
    }

    uint64_t LinuxPwm::frequencyToPeriod(const uint32_t frequencyHz) {
        return kNanosecondsPerSecond / frequencyHz;
    }

}