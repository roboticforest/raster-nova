################################################################################
# Author: David Vitez (AKA: Robotic Forest)
# Copyright: All rights reserved © 2025 David Vitez
# License: This Source Code Form is subject to the terms of the Mozilla Public
#          License, v. 2.0. If a copy of the MPL was not distributed with this
#          file, You can obtain one at https://mozilla.org/MPL/2.0/.
################################################################################

#########################
# Basic Details
#########################

# Cross-platform project!
# Supported platforms:
# - Windows 11.
# - Linux Mint (Ubuntu/Debian based systems).

cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(raster-nova VERSION 0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

#########################
# Dependencies
#########################

# SDL3 (Version 3.3.0 or newer)
add_subdirectory(vendor/SDL3 EXCLUDE_FROM_ALL)

#########################
# Build Settings
#########################

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS inc/*.h)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

if (WIN32)
    # Prevent a console window from opening.
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

#########################
# Linking Settings
#########################

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)