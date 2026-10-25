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
    canvas->set_pixel(0, 0, Color{255, 0, 0, 255});
    canvas->set_pixel(1, 0, Color{0, 255, 0, 255});
    canvas->set_pixel(2, 0, Color{0, 0, 255, 255});
    canvas->set_pixel(29, 17, Color{255, 0, 0, 255});
    canvas->set_pixel(30, 17, Color{0, 255, 0, 255});
    canvas->set_pixel(31, 17, Color{0, 0, 255, 255});

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
