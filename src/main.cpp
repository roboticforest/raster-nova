/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2025 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <exception>
#include <memory>

import canvas;

/** @brief Creates the display resources and runs the pixel demo. SDL must be initialized. */
static int run_app() {
    std::shared_ptr<SDL_Window> window;
    std::shared_ptr<SDL_Renderer> renderer;
    std::unique_ptr<Canvas> canvas;
    try {
        window = {SDL_CreateWindow("Raster Nova", 1600, 900, 0), SDL_DestroyWindow};
        if (!window) {
            SDL_Log("Could not create window: %s", SDL_GetError());
            return 1;
        }

        // Keep the window alive until the renderer has been destroyed.
        renderer = {SDL_CreateRenderer(window.get(), nullptr), [window](SDL_Renderer* renderer) {
            SDL_DestroyRenderer(renderer);
        }};
        if (!renderer) {
            SDL_Log("Could not create renderer: %s", SDL_GetError());
            return 1;
        }

        canvas = std::make_unique<Canvas>(renderer, 32, 18);
    } catch (const std::exception& error) {
        SDL_Log("Could not allocate display resources: %s", error.what());
        return 1;
    }

    // Draw at bounding corners to test both set_pixel() and texture scaling.
    canvas->set_pixel(0, 0, colors::red());
    canvas->set_pixel(1, 0, colors::lime());
    canvas->set_pixel(2, 0, colors::blue());
    canvas->set_pixel(29, 17, colors::red());
    canvas->set_pixel(30, 17, colors::lime());
    canvas->set_pixel(31, 17, colors::blue());

    // Alpha blending test.
    canvas->set_pixel(1, 1, {0,0,255,127});
    canvas->set_pixel(1, 1, {255,0,0,127});

    canvas->use_centered_coordinates();
    canvas->set_pixel(0,0, colors::white());
    canvas->set_pixel(-1,-1, colors::orange());
    canvas->set_pixel(1,1, colors::red());
    canvas->set_pixel(2,2, colors::dark_red());

    // Draw measurement lines.
    const Color measurement_color{255, 255, 255, 128};
    for (int x = -16; x < 16; ++x) {
        if (x % 2 == 0 || x % 5 == 0) {
            canvas->set_pixel(x, 0, measurement_color);
        }
        if (x % 5 == 0) {
            canvas->set_pixel(x, 0, measurement_color);
        }
    }
    for (int y = -9; y < 9; ++y) {
        if (y % 2 == 0 || y % 5 == 0) {
            canvas->set_pixel(0, y, measurement_color);
        }
        if (y % 5 == 0) {
            canvas->set_pixel(0, y, measurement_color);
        }
    }

    canvas->set_pixel(-1.0, -1.0, {0,255,0,127});
    canvas->set_pixel(1.0, 1.0, {0,255,0,127});

    int exit_code = 0;
    while (true) {
        if (!canvas->show()) {
            SDL_Log("Could not show canvas: %s", SDL_GetError());
            exit_code = 1;
            break;
        }

        SDL_Event event;
        if (!SDL_WaitEvent(&event)) {
            SDL_Log("Could not wait for an event: %s", SDL_GetError());
            exit_code = 1;
            break;
        }
        if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            break;
        }
    }

    return exit_code;
}

/** @brief Displays test canvas pixels until the window closes. Returns nonzero on failure. */
int main(int num_args, char** args) {
    (void)num_args; // Ignored.
    (void)args;     // Ignored.

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Could not initialize SDL: %s", SDL_GetError());
        return 1;
    }

    const int exit_code = run_app();
    SDL_Quit();
    return exit_code;
}
