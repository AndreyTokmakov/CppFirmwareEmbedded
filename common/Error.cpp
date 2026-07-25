/**============================================================================
Name        : Error.cpp
Created on  :
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description :
============================================================================**/

#include "Error.hpp"


#include <string_view>

namespace common
{
    using namespace std::string_view_literals;

    [[nodiscard]]
    constexpr std::string_view toString(const Error error)
    {
        switch (error)
        {
            case Error::Success: return "None"sv;
            case Error::NotSupported: return "NotSupported"sv;
            case Error::InvalidParameter: return "InvalidParameter"sv;
            case Error::OutOfRange: return "OutOfRange"sv;
            case Error::HardwareFailure: return "HardwareFailure"sv;
            case Error::Busy: return "Busy"sv;
            case Error::NotInitialized: return "NotInitialized"sv;
            case Error::NotReady: return "NotReady"sv;
            case Error::Timeout: return "Timeout"sv;
            case Error::InternalError: return "InternalError"sv;
            case Error::ResourceUnavailable: return "ResourceUnavailable"sv;
        }
        return "Unknown"sv;
    }
}