# Project Raster Nova

Before tagging a release milestone, update the `project()` version in
`CMakeLists.txt` to match the new Git tag.

## Build and Installation
To install and build this project you need to:
- Clone this repo.
- Clone SDL3 (version 3.3.0 or newer) into a `vendor` directory.
- Be using build tools that support C++20 modules.

Use the following clone commands:
```bash
`git clone https://github.com/roboticforest/raster-nova.git`
cd raster-nova
`git clone https://github.com/libsdl-org/SDL.git vendor/SDL3`
```

## Tests

Build normally, then run the single test executable through CTest:

```sh
cmake --build build/debug
ctest --test-dir build/debug --output-on-failure
```

All automated cases live in `tests/main.cpp`. Add checks there; no CMake changes
are needed for new cases. Each failed check prints a description, and pixel checks
also print the coordinates and expected and actual RGBA values. The program returns
nonzero if any check fails. Checks remain active in both Debug and Release builds.

The tests use a software renderer without opening a window. Keep using the regular
application for visual checks of window presentation and scaling. Configure with
`-DBUILD_TESTING=OFF` to omit the test executable.

## Cross-platform Support
Tested and run on:
- Windows 11
- Linux Mint 20.3 Cinnamon

## Dependencies
### Simple DirectMedia Layer 3 (SDL3)
If you are having any troubles with the above `git clone` command, detailed installation instructions for SDL3 are on [GitHub](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md), and full documentation for SDL3 is on their [official wiki site](https://wiki.libsdl.org/SDL3/FrontPage).

### Development Tools
This project was developed for both Windows 11 and Linux Mint 20.3 using CLion. You can use any IDE you like (or none at all) as long as your build tools support C++20 modules and your editor handles the new syntax correctly. If you do use CLion, you may need to manually update or replace its bundled toolchain like I did.

Everything was built and tested with the following software versions (or newer).

#### Common Between OSes
- CLion 2023.2 (runtime 17.0) IDE.
- CMake 3.31 Build System.

#### Windows
- MSVC 19.44 (Visual Studio 17 2022) Compiler
- MSVC 19.44 (Visual Studio 17 2022) Build Generator

#### Linux
- Clang 18.1 Compiler.
- Ninja 1.11 Build Generator.

## License
This software is provided under the Mozilla Public License Version 2.0 (MPL 2.0). See [LICENSE.txt](LICENSE.txt), or visit the [Mozilla Public License](https://mozilla.org/MPL/2.0/) website for full details. The following summary is ***not*** a substitute for the full license.

MPL 2.0 allows you to freely use, modify, and distribute this software, even for commercial projects. If you modify any part of this software, you must make those changes available under MPL 2.0. You're not required to open-source your entire project, just the modified files of the original software.

You must keep all copyright, patent, trademark, and attribution notices intact when you distribute or display any part of this software.

No warranties are provided, and the creators aren't liable for damages.

The license also grants you the right to use patents from contributors to the original software, but this doesn't extend to modified or combined software.
