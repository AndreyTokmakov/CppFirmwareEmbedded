cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 26)

project(hal)
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_compile_options(-c -Wall -O3 -std=c++26)

add_executable(${PROJECT_NAME}
        main.cpp
        include/ISpiBus.hpp
        include/IGpio.hpp
        include/ITimer.hpp
        include/IDelay.hpp
        include/II2cBus.hpp
        include/IUart.hpp
        include/IAdc.hpp
        include/IDac.hpp
        include/IClock.hpp
        include/IPwm.hpp
        linux/LinuxSpiBus.cpp
        linux/LinuxGpio.cpp
        linux/LinuxUart.hpp
        linux/LinuxUart.cpp
        linux/LinuxI2cBus.hpp
        linux/LinuxI2cBus.cpp
        linux/LinuxPwm.cpp
        linux/LinuxPwm.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/common")
target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/common")

#[[
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/utilities")
target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/utilities")
]]#

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        common_lib
        # utilities_lib
        pthread
        gpiod
        ${EXTRA_LIBS}
)
