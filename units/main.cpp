/**============================================================================
Name        : main.cpp
Created on  : 
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description :
============================================================================**/

#include <string_view>
#include <vector>


int main([[maybe_unused]] const int argc,
         [[maybe_unused]] char** argv)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    return EXIT_SUCCESS;
}

/**

units/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── docs/
│   ├── design.md
│   ├── naming.md
│   ├── conventions.md
│   └── roadmap.md
│
├── include/
│   └── units/
│       ├── constants.hpp
│       │
│       ├── detail/
│       │   ├── relative_quantity.hpp
│       │   ├── absolute_quantity.hpp
│       │   ├── floating_point.hpp
│       │   └── concepts.hpp
│       │
│       ├── frequency.hpp
│       ├── duration.hpp
│       ├── voltage.hpp
│       ├── current.hpp
│       ├── resistance.hpp
│       ├── power.hpp
│       ├── energy.hpp
│       ├── charge.hpp
│       ├── temperature.hpp
│       ├── pressure.hpp
│       ├── length.hpp
│       ├── mass.hpp
│       ├── angle.hpp
│       ├── time.hpp
│       └── ...
│
├── tests/
│   ├── CMakeLists.txt
│   │
│   ├── framework/
│   │   ├── assert.hpp
│   │   ├── test_runner.hpp
│   │   └── test_runner.cpp
│   │
│   ├── detail/
│   │   └── test_relative_quantity.cpp
│   │
│   ├── frequency/
│   │   └── frequency_test.cpp
│   │
│   ├── voltage/
│   │   └── voltage_test.cpp
│   │
│   ├── current/
│   │   └── current_test.cpp
│   │
│   ├── resistance/
│   │   └── resistance_test.cpp
│   │
│   └── ...
│
├── examples/
│   ├── frequency.cpp
│   ├── electrical.cpp
│   ├── rf.cpp
│   ├── firmware.cpp
│   └── dsp.cpp
│
├── benchmarks/
│   ├── frequency.cpp
│   ├── voltage.cpp
│   └── arithmetic.cpp
│
└── cmake/

**/