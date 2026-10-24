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

import canvas;

/** @brief Displays test canvas pixels until the window closes. Returns nonzero on failure. */
int main(int num_args, char** args) {
    (void)num_args; // Ignored.
    (void)args;     // Ignored.

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Could not initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Raster Nova", 800, 600, 0);
    if (window == nullptr) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int exit_code = 0;
    try {
        Canvas canvas(*renderer, 200, 150);
        canvas.set_pixel(99, 75, Color{255, 0, 0, 255});
        canvas.set_pixel(100, 75, Color{0, 255, 0, 255});
        canvas.set_pixel(101, 75, Color{0, 0, 255, 255});

        while (true) {
            if (!canvas.show()) {
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
    } catch (const std::exception& error) {
        SDL_Log("Could not allocate canvas: %s", error.what());
        exit_code = 1;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return exit_code;
}
