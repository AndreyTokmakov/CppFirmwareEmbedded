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

namespace rf
{
    /**
     * @brief Generic result codes returned by RF devices.
     */
    enum class [[nodiscard]] Error
    {
        None,
        /// Requested operation is not supported by the device.
        NotSupported,
        /// Parameter is outside the supported range.
        InvalidParameter,
        /// Device is currently busy.
        Busy,
        /// Communication with the hardware failed.
        CommunicationError,
        /// Device is not initialized.
        NotInitialized,
        /// Internal device error.
        HardwareError
    };
}

#endif //CPPFIRMWAREEMBEDDED_ERROR_HPP
