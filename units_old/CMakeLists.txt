cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 26)

project(units_old)
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_compile_options(-c -Wall -O3 -std=c++26)

add_library(units_lib
        frequency.cpp
        frequency.hpp
        voltage.hpp
        temperature.hpp
)

add_executable(${PROJECT_NAME}
        main.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        units_lib
        pthread
        ${EXTRA_LIBS}
)