################################################################################
# Author: David Vitez (AKA: Robotic Forest)
# Copyright: All rights reserved © 2025 David Vitez
# License: This Source Code Form is subject to the terms of the Mozilla Public
#          License, v. 2.0. If a copy of the MPL was not distributed with this
#          file, You can obtain one at https://mozilla.org/MPL/2.0/.
################################################################################

cmake_minimum_required(VERSION 4.0)
project(raster-nova VERSION 0.5.0)

message("╔════════════════════════════════════════════════════════════════════╗")
message("║ RASTER NOVA                                                        ║")
message("╚════════════════════════════════════════════════════════════════════╝")

################################################################################
# Basic Details

# Cross-platform project!
# Supported platforms:
# - Windows 11.
# - Linux Mint (Ubuntu/Debian based systems).

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

################################################################################
# Dependencies

# Simple DirectMedia Layer 3 (SDL3).
message(CHECK_START "SDL3 source installed as vendor")
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/SDL3/CMakeLists.txt")
    message(CHECK_FAIL "NOT installed")
    message(FATAL_ERROR "SDL3 source not found!
    SDL3 (version 3.3.0 or higher) is required! Expected at: ${CMAKE_SOURCE_DIR}/vendor/SDL3
    To install, run: `git clone https://github.com/libsdl-org/SDL.git vendor/SDL3`
    For more details visit SDL3's official GitHub at:
    https://github.com/libsdl-org/SDL/blob/main/INSTALL.md
    and follow the CMake installation instructions."
    )
else ()
    message(CHECK_PASS "installed")
endif()

message("● SDL3 - Start of CMake Output ▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬")
add_subdirectory(vendor/SDL3 EXCLUDE_FROM_ALL)

################################################################################
# Build Settings

add_library(raster-nova-core STATIC)
target_sources(raster-nova-core
        PRIVATE
        src/color.cpp
        src/canvas.cpp

        PUBLIC FILE_SET cpp_modules
        TYPE CXX_MODULES
        FILES
        src/color.cppm
        src/canvas.cppm
)

add_executable(${PROJECT_NAME} src/main.cpp)

include(CTest)
if (BUILD_TESTING)
    add_executable(raster-nova-tests tests/main.cpp)
    target_link_libraries(raster-nova-tests PRIVATE raster-nova-core)
    add_test(NAME canvas COMMAND raster-nova-tests)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("● Linux OS - Start of CMake Output ▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬")
    message(STATUS "None needed currently.")
endif ()

if (WIN32)
    message("● Windows OS - Start of CMake Output ▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬")
    message(STATUS "Suppress SDL3 debug console window.")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

################################################################################
# Linking Settings

target_link_libraries(raster-nova-core PUBLIC SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE raster-nova-core)

if (WIN32 AND TARGET SDL3::SDL3-shared)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3-shared>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    if (BUILD_TESTING)
        add_custom_command(TARGET raster-nova-tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL3::SDL3-shared>
                $<TARGET_FILE_DIR:raster-nova-tests>
        )
    endif ()
endif ()

message("╔════════════════════════════════════════════════════════════════════╗")
message("║ RASTER NOVA - CMake Config Complete                                ║")
message("╚════════════════════════════════════════════════════════════════════╝")
