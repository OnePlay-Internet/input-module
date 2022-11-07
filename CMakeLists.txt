cmake_minimum_required(VERSION 3.20)
project(input-module)

set(inputmodule_SOURCE_FILES 
    third-party/moonlight-common-c/src/Input.h
    src/input.cpp
)

add_library(inputmodule STATIC
    ${inputmodule_SOURCE_FILES}
)

target_include_directories(inputmodule PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
