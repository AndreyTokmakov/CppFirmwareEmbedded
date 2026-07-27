cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 26)

project(attenuation)
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_compile_options(-c -Wall -O3 -std=c++26)

add_executable(${PROJECT_NAME}
        main.cpp
        IAttenuator.hpp
        AttenuatorBase.hpp
        FixedAttenuator.hpp
        FixedAttenuator.cpp
        DigitalStepAttenuator.hpp
        DigitalStepAttenuator.cpp
        SpiAttenuator.hpp
        SpiAttenuator.cpp
        ParallelAttenuator.hpp
        ParallelAttenuator.cpp

        HMC624.hpp
        HMC624.cpp
        PE4312.hpp
        PE4312.cpp
        PE43711.hpp
        PE43711.cpp
        ADRF5700.cpp
        ADRF5700.hpp
        # AD9361RxAttenuator.hpp
        # AD9361RxAttenuator.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/common")
target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/common")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/utilities")
target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/utilities")

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        common_lib
        utilities_lib
        pthread
        ${EXTRA_LIBS}
)