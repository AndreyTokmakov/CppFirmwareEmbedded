cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 26)

project(units)
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

add_compile_options(-c -Wall -O3 -std=c++26)

add_executable(${PROJECT_NAME}
    main.cpp
    include/frequency.hpp
    include/constants.hpp
    include/voltage.hpp
    include/temperature.hpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    pthread
    ${EXTRA_LIBS}
)

add_subdirectory(tests)