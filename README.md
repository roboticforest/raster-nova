# Project Gold Cartridge: SDL2 Starter Template

## Overview

Jump-start your Simple Direct-Media Layer projects with this foundational code base.

The aim of this project is to provide a simplified starting point for developers new to SDL2, or for anyone that just wants to quickly test an idea without the hassle of setup and configuration.

In short, this is SDL2 boilerplate, with extras.

### Features

- **Quick To Start:** Simply click the "Use this template" button to make copy of this repo, then edit the provided callback functions in `main.cpp`. This should run on Windows and Linux with no further setup.
- **Simplified Initialization:** SDL2, SDL_image, and SDL_ttf are already configured and initialized. A simple `Window` class runs all of your code, provides a renderer, and handles window close events. You may optionally supply your own event handling code to deal with keyboard or mouse input.
- **Fixed Update Rate:** Your update callback runs at a fixed 60Hz, ensuring smooth and consistent behavior for your simulations and games. Update rates and permissible frame drops are customizable.
- **Font Rendering:** Includes very simple font rendering and text display with a default font provided.
- **Standard Colors:** Provides a collection of standard web colors for your convenience, all provided as SDL_Color structures.
- **Simple UI Elements:** Includes a basic button object for creating interactive UI elements.

### Prerequisites

- **C++:** This project uses the C++20 language standard.

- **CMake:** Version 3.19 or later.

- **Simple DirectMedia Layer 2:** SDL2, SDL_image, SDL_ttf, SDL_net, and SDL_mixer should already be installed on your machine. For best results use default installation locations.

## Getting Started

1. Run `git clone git@github.com:roboticforest/gold-cartridge.git`.
2. Open in a C++ IDE with CMake support.
3. Build and run (you should see an empty window).
4. Edit `main.cpp` to your liking.

If you need/prefer to use a Linux-style terminal:

```bash
git clone git@github.com:roboticforest/gold-cartridge.git
cd gold_cartridge
mkdir build
cd build
cmake ..
cmake --build .
```

## Project Structure

This template repo is intended to get simple projects up and running in minutes or less. Basic SDL2 configuration and initialization are handled with placeholders marked out for your own code.

If you wish to do something more complex, you are free to ignore, modify, or tweak any of the provided files, functions, or classes to suit your needs.

### What Is Where

- `main.cpp`
    - Program entry point, and where your code is expected to go.

    - SDL2 and Window start-up occur here. Empty callback functions for input handling, updating, and drawing are provided for you to fill in.

    - Most quick or simple projects are expected to only need to edit this one file. Feel free to create as many, or as few, supporting code files as you need though.

- `core/`
    - `System.h`: A singleton manager class that initializes SDL2 and all other managers.
    - `FontManager.h`: A singleton manager class for loading and rendering fonts.

- `rendering/`
    - `Windowing.h`: A basic Window class. It manages the internal "game loop", managing timing, calls upon your supplied processing and drawing code, and provides access to an internal SDL_Renderer.
    - `Colors.h`: Provides web colors for convenience.

- `ui/`
    - `Button.h`: A basic Button class.

- `resources/`
  - `fonts/`: Holds the default font.
  - `img/`: Holds a sample image you can use for rendering tests.

## Contribute

Open-source under MPL 2.0. See [issues](https://github.com/roboticforest/gold-cartridge/issues) for any open tasks.

## License

This software is provided under the Mozilla Public License Version 2.0 (MPL 2.0). See [LICENSE.txt](LICENSE.txt), or visit the [Mozilla Public License](https://mozilla.org/MPL/2.0/) website for full details. The following summary is ***not*** a substitute for the full license.

MPL 2.0 allows you to freely use, modify, and distribute this software, even for commercial projects. If you modify any part of this software, you must make those changes available under MPL 2.0. You're not required to open-source your entire project, just the modified files of the original software.

You must keep all copyright, patent, trademark, and attribution notices intact when you distribute or display any part of this software.

No warranties are provided, and the creators aren't liable for damages.

The license also grants you the right to use patents from contributors to the original software, but this doesn't extend to modified or combined software.
