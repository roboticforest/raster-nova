/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2025 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/** @brief Opens a window and waits for a close event. Returns nonzero on failure. */
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

    int exit_code = 0;
    SDL_Event event;
    while (true) {
        if (!SDL_WaitEvent(&event)) {
            SDL_Log("Could not wait for an event: %s", SDL_GetError());
            exit_code = 1;
            break;
        }
        if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return exit_code;
}
