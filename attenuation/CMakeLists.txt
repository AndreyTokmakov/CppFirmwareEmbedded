cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 26)

project(attenuation)
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_compile_options(-c -Wall -O3 -std=c++26)

add_executable(${PROJECT_NAME}
        main.cpp
        Error.hpp
        IAttenuator.hpp
        AttenuatorBase.hpp
        FixedAttenuator.hpp
        FixedAttenuator.cpp
        PE4312.hpp
        PE4312.cpp
        AD9361RxAttenuator.hpp
        AD9361RxAttenuator.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        pthread
        ${EXTRA_LIBS}
)