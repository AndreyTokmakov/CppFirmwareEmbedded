/**============================================================================
Name        : Error.hpp
Created on  : 24.07.2026
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : Error.hpp
============================================================================**/

#ifndef CPPFIRMWAREEMBEDDED_ERROR_HPP
#define CPPFIRMWAREEMBEDDED_ERROR_HPP

#include <cstdint>
#include <string_view>

namespace common
{
    /**
     * Generic result codes returned by RF devices.
     */
    enum class [[nodiscard]] Error : uint8_t
    {
        /**
         * Operation completed successfully.
         */
        Success = 0,

        /**
         * Requested operation is not supported by the device.
         *
         * Example:
         * Fixed attenuator:
         *     SetAttenuation() is not supported.
         */
        NotSupported,

        /**
         * Invalid parameter was provided.
         *
         * Examples:
         *   - incorrect GPIO number
         *   - invalid SPI mode
         *   - unsupported frequency
         */
        InvalidParameter,

        /**
         * Requested value is outside the supported range.
         *
         * Examples:
         *   PE4312:
         *       SetAttenuation(50.0) while maximum attenuation is 31.5 dB.
         */
        OutOfRange,

        /**
         * Hardware communication failed.
         *
         * Examples:
         *   - SPI transaction error
         *   - I2C communication failure
         *   - GPIO access error
         */
        HardwareFailure,

        /**
         * Device is currently busy.
         * The operation should be retried later.
         *
         * Example:
         *   PLL frequency update
         *   ADC conversion
         *   RF calibration
         */
        Busy,

        /**
         * Device is not initialized.
         *
         * Operation requires successful initialization first.
         */
        NotInitialized,

        /**
         * Device is not ready for operation.
         *
         * Example:
         *   - PLL not locked
         *   - ADC calibration not completed
         */
        NotReady,

        /**
         * Timeout occurred during operation.
         *
         * Examples:
         *   - SPI transaction timeout
         *   - waiting for PLL lock
         *   - ADC conversion timeout
         */
        Timeout,

        /**
         * Internal software error.
         * Indicates an unexpected condition inside the driver.
         */
        InternalError,

        /**
         * Memory allocation or resource allocation failure.
         *
         * Example:
         *
         *   - unable to allocate buffer
         *   - resource unavailable
         */
        ResourceUnavailable
    };

    /**
     * Converts Error value into human-readable string.
     * This helper is useful for logging and debugging.
     *
     * Example:
     *     std::cout << toString(Error::Timeout);
     *
     * @param error Error value.
     * @return Text representation of the error.
     */
    [[nodiscard]]
    constexpr std::string_view toString(const Error error);
}

#endif //CPPFIRMWAREEMBEDDED_ERROR_HPP
