/**============================================================================
Name        : LinuxPwm.hpp
Created on  : 26.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : LinuxPwm.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_LINUXPWM_HPP
#define CPPFIRMWAREEMBEDDED_LINUXPWM_HPP

#include <cstdint>
#include <filesystem>
#include <string>

#include "../include/IPwm.hpp"

namespace hal
{
    /**
     * Linux implementation of the IPwm interface.
     *
     * Overview
     * --------
     *
     * This class provides a Linux-specific implementation of the generic PWM
     * interface using the Linux PWM sysfs subsystem.
     *
     * PWM (Pulse Width Modulation) is a hardware feature commonly implemented by timer peripherals inside SoCs
     * and microcontrollers. The Linux kernel exposes these hardware PWM channels through the PWM framework, allowing
     * user-space applications to configure and control them without direct  access to hardware registers.
     *
     * The purpose of this class is to hide Linux-specific implementation details behind the platform-independent
     * IPwm interface so that higher software layers remain completely portable.
     *
     * Linux PWM Subsystem
     * ---------------------------------
     *
     * Linux exposes PWM controllers as pwmchip devices:
     *
     *     /sys/class/pwm/
     *         pwmchip0/
     *         pwmchip1/
     *         ...
     *
     * Every PWM controller provides one or more output channels.
     *
     * Before a channel can be configured, it must be exported:
     *     echo 0 > export
     *
     * After exporting, Linux creates a directory:
     *     pwm0/
     *
     * containing control files:
     *
     *     enable
     *     period
     *     duty_cycle
     *     polarity
     *
     * Configuration is performed by writing ASCII values into these files.
     *
     * Hardware Independence
     * ---------------------------------
     *
     * This implementation never accesses timer registers directly.
     *
     * The Linux kernel is responsible for communicating with the underlying hardware driver.
     * The same user-space API therefore works for many different SoCs including:
     *
     *     • NXP i.MX
     *     • Raspberry Pi
     *     • TI Sitara
     *     • Xilinx Zynq
     *     • Rockchip
     *     • Allwinner
     *     • STM32MP1
     *
     *
     * Typical Applications
     * ---------------------------------
     *
     * PWM is commonly used for:
     *
     *     • Power amplifier bias generation
     *     • RF switch timing
     *     • Clock enable signals
     *     • Fan speed control
     *     • LED brightness control
     *     • Motor control
     *     • Analog voltage generation using an RC filter
     *
     *
     * Timing Model
     * ---------------------------------
     *
     * A PWM signal is defined by:
     *     Period     : Total duration of one PWM cycle.
     *     Duty Cycle : Fraction of the period during which the output is HIGH.
     *     Frequency  : Reciprocal of the period.
     *
     * Linux internally represents both period and duty cycle in nanoseconds. The IPwm interface instead exposes
     * frequency in Hertz and duty cycle as a floating-point value in the range [0.0, 1.0].
     *
     * Resource Management
     * ---------------------------------
     *
     * The PWM channel is exported during construction if necessary and  automatically unexported when the object is destroyed.
     * The implementation follows the RAII principle to ensure that all resources are released correctly.
     *
     * Thread Safety
     * ---------------------------------
     *
     * LinuxPwm instances are not internally synchronized.
     *
     * If multiple threads access the same PWM channel simultaneously,
     * synchronization must be provided by the caller.
     *
     * Dependencies
     * ---------------------------------
     *
     * Required Linux kernel support:
     *     CONFIG_PWM
     *     CONFIG_SYSFS
     *
     * Required filesystem:
     *     /sys/class/pwm
     *
     * Limitations
     * ---------------------------------
     *
     * This implementation controls only one PWM channel.
     *
     * It assumes that the underlying Linux driver supports changing both
     * period and duty cycle while the channel is enabled.
     *
     * Some hardware drivers require disabling the channel before changing these parameters.
     * Such restrictions are handled transparently by the Linux kernel driver.
     *
     * Example
     * ---------------------------------
     *     LinuxPwm pwm("/sys/class/pwm/pwmchip0", 0);
     *     pwm.SetFrequency(1000);
     *     pwm.SetDutyCycle(0.5);
     *     pwm.Enable();
     */
    class LinuxPwm : public IPwm
    {
    public:

        /**
         * Opens a PWM channel.
         * If the requested channel has not yet been exported, the constructor exports it automatically.
         *
         * @param pwmChip Path to the pwmchip directory.
         * Example:
         *     "/sys/class/pwm/pwmchip0"
         * @param channel PWM channel index.
         */
        LinuxPwm(const std::filesystem::path& pwmChip, uint32_t channel);

        /**
         * Destroys the PWM object.
         *
         * The destructor performs no communication with the operating system.
         * If the PWM channel has been opened, Close() should be called explicitly before object destruction.
         */
        ~LinuxPwm() override = default;

        /**
         * Initializes the PWM channel.
         *
         * The method exports the requested PWM channel if necessary and applies the currently configured frequency and duty cycle.
         * Calling Open() multiple times is safe.
         *
         * @return Operation status.
         */
        Error open();

        /**
         * Releases the PWM channel.
         *
         * The PWM output is disabled and the channel is unexported if it was exported by this object.
         * Calling Close() multiple times is safe.
         *
         * @return Operation status.
         */
        Error close();

        /**
         * Enables PWM signal generation.
         *
         * @return Operation status.
         */
        Error enable() override;

        /**
         * Disables PWM signal generation.
         *
         * @return Operation status.
         */
        Error disable() override;

        /**
         * Sets the PWM output frequency.
         * Internally this method converts frequency to the corresponding PWM period expressed in nanoseconds.
         *
         * @param frequencyHz Desired PWM frequency.
         * @return Operation status.
         */
        Error setFrequency(uint32_t frequencyHz) override;

        /**
         * Sets the PWM duty cycle.
         *
         * The duty cycle is expressed as a normalized value in the range from 0.0 to 1.0.
         *     0.0 -> Always LOW
         *     0.5 -> 50% duty cycle
         *     1.0 -> Always HIGH
         *
         * @param dutyCycle Desired duty cycle.
         * @return Operation status.
         */
        Error setDutyCycle(double dutyCycle) override;

        /**
         * Returns the configured PWM frequency.
         *
         * @return Frequency in Hertz.
         */
        [[nodiscard]]
        uint32_t getFrequency() const noexcept override;

        /**
         * Returns the configured duty cycle.
         *
         * @return Duty cycle in the range [0.0, 1.0].
         */
        [[nodiscard]]
        double getDutyCycle() const noexcept override;

        /**
         * Returns true if the PWM channel has been successfully opened.
         *
         * @return Initialization state.
         */
        [[nodiscard]]
        bool isOpen() const noexcept ;

    private:

        /**
         * Exports the PWM channel through the Linux PWM subsystem.
         *
         * If the channel has already been exported by another process, the operation succeeds without modifying its ownership.
         *
         * @return Operation status.
         */
        Error exportChannel();

        /**
         * Unexports the PWM channel.
         *
         * The channel is unexported only if it was originally exported by this object.
         * @return Operation status.
         */
        Error unexportChannel() const;

        /**
         * Writes an ASCII value into one of the Linux PWM sysfs files.
         *
         * @param path Path to the target sysfs file.
         * @param value Value to be written.
         * @return Operation status.
         */
        static Error writeFile(const std::filesystem::path& path,
                                std::string_view value);

        /**
         * Converts a PWM frequency expressed in Hertz into a period expressed in nanoseconds.
         *
         * Linux PWM drivers expect the period to be specified in nanoseconds.
         *
         * @param frequencyHz PWM frequency.
         * @return PWM period in nanoseconds.
         */
        [[nodiscard]]
        static uint64_t frequencyToPeriod(uint32_t frequencyHz);

    private:

        std::filesystem::path pwmChipPath;
        std::filesystem::path pwmPath;
        uint32_t channel = 0;
        uint32_t frequency = 1000;
        double dutyCycle = 0.5;
        bool exported = false;
        bool opened = false;
    };

} // namespace hal

#endif //CPPFIRMWAREEMBEDDED_LINUXPWM_HPP
