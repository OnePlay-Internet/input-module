cmake_minimum_required(VERSION 3.20)
project(input-module)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TEST "Build tests" ON)

if(NOT APPLE)
	set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "BOOST_INCLUDE_DIRS: ${Boost_INCLUDE_DIR}")

set(inputmodule_SOURCE_FILES 
    src/config.cpp
    src/config.h
    src/input_impl.cpp
    src/input_impl.h
    src/input_module.cpp
    src/main.h
    src/main.cpp
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
        src/platform/windows/misc.cpp
        third-party/ViGEmClient/src/ViGEmClient.cpp
        third-party/ViGEmClient/include/ViGEm/Client.h
        third-party/ViGEmClient/include/ViGEm/Common.h
        third-party/ViGEmClient/include/ViGEm/Util.h
        third-party/ViGEmClient/include/ViGEm/km/BusShared.h
        )
    
    set_source_files_properties(third-party/ViGEmClient/src/ViGEmClient.cpp PROPERTIES COMPILE_DEFINITIONS "UNICODE=1;ERROR_INVALID_DEVICE_OBJECT_PARAMETER=650")
if(NOT MSVC)
    set_source_files_properties(third-party/ViGEmClient/src/ViGEmClient.cpp PROPERTIES COMPILE_FLAGS "-Wno-unknown-pragmas -Wno-misleading-indentation -Wno-class-memaccess")
endif()

    set(inputmodule_INCLUDE_DIRS
        third-party/ViGEmClient/include)

    set(inputmodule_PLATFORM_LIBS
        setupapi)
endif()

set(CMAKE_DEBUG_POSTFIX -debug)

add_library(inputmodule STATIC
${inputmodule_SOURCE_FILES}
${inputmodule_PLATFORM_FILES})
add_library(inputmodule::inputmodule ALIAS inputmodule)
target_include_directories(inputmodule PRIVATE
    ${Boost_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${inputmodule_INCLUDE_DIRS})
target_link_libraries(inputmodule PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${inputmodule_PLATFORM_LIBS})
set_target_properties(inputmodule PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

if(MSVC)
    target_compile_definitions(inputmodule PRIVATE NOMINMAX)
    # Issue: https://github.com/microsoft/vcpkg/discussions/22762
    target_compile_definitions(inputmodule PRIVATE _WIN32_WINNT=0x0601)
endif()

# Install
# set(CMAKE_INSTALL_PREFIX CACHE STRING ${CMAKE_CURRENT_SOURCE_DIR})
set(inputmodule_INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(inputmodule_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(inputmodule_INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/cmake)
install(TARGETS inputmodule EXPORT InputModuleTarget DESTINATION ${inputmodule_INSTALL_LIB_DIR})
install(FILES include/input_module.h include/input_packet.h DESTINATION ${inputmodule_INSTALL_INCLUDE_DIR})
install(EXPORT InputModuleTarget
        FILE InputModuleTarget.cmake
        NAMESPACE inputmodule::
        DESTINATION ${inputmodule_INSTALL_CMAKE_DIR}
)

# Test depends on libmatoya (https://github.com/parsec-cloud/libmatoya)
# Skip if using gcc on Windows
if(${BUILD_TEST} AND NOT MINGW)
    set(libmatoya_ROOT_DIR "../libmatoya" CACHE STRING "libmatoya source location")
    if(NOT IS_ABSOLUTE ${libmatoya_ROOT_DIR})
        cmake_path(SET libmatoya_ROOT_DIR NORMALIZE "${CMAKE_CURRENT_SOURCE_DIR}/${libmatoya_ROOT_DIR}")
    endif()
    message(STATUS "libmatoya_ROOT_DIR: ${libmatoya_ROOT_DIR}")
    if(MSVC)
        set(libmatoya ${libmatoya_ROOT_DIR}/bin/windows/x64/matoya.lib)
    endif()

    add_subdirectory(tests)
endif()