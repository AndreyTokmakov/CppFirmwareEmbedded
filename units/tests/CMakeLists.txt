cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 26)

project(units_tests)
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_compile_options(-c -Wall -O3 -std=c++26)

add_executable(${PROJECT_NAME}
    all_tests.cpp
    frequency_test.cpp
    voltage_test.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "${INCLUDE_DIR}")

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    pthread
    ${EXTRA_LIBS}
)