# Author: David Vitez (AKA: Robotic Forest)
# Copyright: All rights reserved © 2023 David Vitez
# License: This Source Code Form is subject to the terms of the Mozilla Public
#          License, v. 2.0. If a copy of the MPL was not distributed with this
#          file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.19)
project(gold_cartridge)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp
        core/System.cpp
        core/System.h
        rendering/Windowing.cpp
        rendering/Windowing.h
        rendering/Colors.h
        ui/Button.cpp
        ui/Button.h
        core/FontManager.cpp
        core/FontManager.h)

# button.cpp test_logging.cpp test_asserts.cpp sdl2_loading.cpp globals.cpp test_application.cpp test_application.h

add_executable(gold_cartridge ${SOURCE_FILES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

if (WIN32)
    # SDL2_BINDIR and SDL2_LIBDIR only exist under Win 11. Linux Mint doesn't define/need them.
    target_link_directories(gold_cartridge PUBLIC ${SDL2_BINDIR} ${SDL2_LIBDIR})

    # Win 11 displays a console _window for SDL2 programs. Useful for debugging, annoying for release.
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Debug build on Windows. Allowing console to be visible.")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        message(STATUS "Release build on Windows. Hiding console.")
        # The following is more portable than setting -mwindows manually.
        # Setting the WIN32_EXECUTABLE property will make CMake automatically pass "-mwindows" to GCC/MinGW, and
        # "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup" to MSVC.
        set_target_properties(gold_cartridge PROPERTIES WIN32_EXECUTABLE 1)
    endif ()

    # To avoid errors when linking SDL2 on Windows the linker flags must be specified in this order:
    # -lmingw32 -lSDL2main -lSDL2 -lAnyOtherSDL2Libs
    if (MINGW)
        message(STATUS "Windows MinGW Build: Adding MinGW linker option.")
        target_link_libraries(gold_cartridge PUBLIC -lmingw32)
    endif ()
endif ()

# On Linux Mint leaving out the -lSDL2main flag works for some reason. (Probably because there's no WinMain handling?)
# Adding it under Linux doesn't hurt anything, provides consistency, and it *is* required on Windows.
target_link_libraries(gold_cartridge PUBLIC -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf)

# Move program resources and needed library files into the build directory.
file(COPY "resources" DESTINATION "${PROJECT_BINARY_DIR}")
if (WIN32)
    message(STATUS "Copying SDL2 Windows DLLs to project build directory.")
    file(COPY "${SDL2_BINDIR}/SDL2.dll" DESTINATION "${PROJECT_BINARY_DIR}")
    file(COPY "${SDL2_BINDIR}/SDL2_image.dll" DESTINATION "${PROJECT_BINARY_DIR}")
    file(COPY "${SDL2_BINDIR}/SDL2_mixer.dll" DESTINATION "${PROJECT_BINARY_DIR}")
    file(COPY "${SDL2_BINDIR}/SDL2_net.dll" DESTINATION "${PROJECT_BINARY_DIR}")
    file(COPY "${SDL2_BINDIR}/SDL2_ttf.dll" DESTINATION "${PROJECT_BINARY_DIR}")
endif ()
