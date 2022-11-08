cmake_minimum_required(VERSION 3.20)
project(input-module)

find_package(Boost COMPONENTS log REQUIRED)

set(inputmodule_SOURCE_FILES 
    third-party/moonlight-common-c/src/Input.h
    src/config.cpp
    src/config.h
    src/input_impl.cpp
    src/input_impl.h
    src/input.cpp
    src/main.h
    src/move_by_copy.h
    src/task_pool.h
    src/thread_pool.h
    src/thread_safe.h
    src/utility.h
    src/platform/common.h
)

if(WIN32)
    set(inputmodule_PLATFORM_FILES
        src/platform/windows/input.cpp
        third-party/ViGEmClient/src/ViGEmClient.cpp
        third-party/ViGEmClient/include/ViGEm/Client.h
        third-party/ViGEmClient/include/ViGEm/Common.h
        third-party/ViGEmClient/include/ViGEm/Util.h
        third-party/ViGEmClient/include/ViGEm/km/BusShared.h
        )
    
    set_source_files_properties(third-party/ViGEmClient/src/ViGEmClient.cpp PROPERTIES COMPILE_DEFINITIONS "UNICODE=1;ERROR_INVALID_DEVICE_OBJECT_PARAMETER=650")
    set_source_files_properties(third-party/ViGEmClient/src/ViGEmClient.cpp PROPERTIES COMPILE_FLAGS "-Wno-unknown-pragmas -Wno-misleading-indentation -Wno-class-memaccess")
endif()

add_library(inputmodule STATIC
    ${inputmodule_SOURCE_FILES}
    ${inputmodule_PLATFORM_FILES}
)

target_include_directories(inputmodule PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
    target_include_directories(inputmodule PRIVATE third-party/ViGEmClient/include)
endif()

target_link_libraries(inputmodule PRIVATE Boost::log)