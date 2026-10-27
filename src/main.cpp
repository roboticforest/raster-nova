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
#include <cmath>

import canvas;

// Can only draw low-angle lines in one positive octant, from horizontal to diagonal.
void naive_draw_line(Canvas* canvas, const std::uint32_t start_x, const std::uint32_t start_y, const std::uint32_t end_x, const std::uint32_t end_y) {
    double delta_x = end_x - start_x;
    double delta_y = end_y - start_y;
    double slope = 0;
    if(delta_x != 0) { // Ensure line isn't vertical.
        slope = double(delta_y / delta_x);
        for(std::uint32_t i = 0; i <= delta_x; ++i) {
            canvas->set_pixel(int(start_x + i), int(std::round(start_y + i * slope)), colors::red());
        }
    }
}

// Can draw all lines in positive octants, from horizontal to vertical.
void better_draw_line(Canvas* canvas, const std::int32_t start_x, const std::int32_t start_y, const std::int32_t end_x, const std::int32_t end_y) {
    double delta_x = end_x - start_x;
    double delta_y = end_y - start_y;
    double step = std::max(std::abs(delta_x), std::abs(delta_y));
    double step_x, step_y = 0;
    if(step != 0) {
        step_x = delta_x / step;
        step_y = delta_y / step;
        for(std::uint32_t i = 0; i <= step; ++i) {
            canvas->set_pixel(int(std::round(start_x + i * step_x)), int(std::round(start_y + i * step_y)), colors::white());
        }
    }
}

/** @brief Creates the display resources and runs the pixel demo. SDL must be initialized. */
static int run_app() {
    // Data
    int window_width = 1600;
    int window_height = 900;
    unsigned int canvas_width = 16 * 4;
    unsigned int canvas_height = 9 * 4;

    std::shared_ptr<SDL_Window> window;
    std::shared_ptr<SDL_Renderer> renderer;
    std::unique_ptr<Canvas> canvas;

    // Setup
    try {
        window = {SDL_CreateWindow("Raster Nova", window_width, window_height, 0), SDL_DestroyWindow};
        if(!window) {
            SDL_Log("Could not create window: %s", SDL_GetError());
            return 1;
        }

        // Keep the window alive until the renderer has been destroyed.
        renderer = {
            SDL_CreateRenderer(window.get(), nullptr), [window](SDL_Renderer* renderer) {
                SDL_DestroyRenderer(renderer);
            }
        };
        if(!renderer) {
            SDL_Log("Could not create renderer: %s", SDL_GetError());
            return 1;
        }

        canvas = std::make_unique<Canvas>(renderer, canvas_width, canvas_height);
    }
    catch(const std::exception& error) {
        SDL_Log("Could not allocate display resources: %s", error.what());
        return 1;
    }

    // Drawing
    canvas->use_centered_coordinates();
    canvas->set_pixel(0, 0, colors::red());

    // Core App Loop
    int exit_code = 0;
    while(true) {
        if(!canvas->show()) {
            SDL_Log("Could not show canvas: %s", SDL_GetError());
            exit_code = 1;
            break;
        }

        SDL_Event event;
        if(!SDL_WaitEvent(&event)) {
            SDL_Log("Could not wait for an event: %s", SDL_GetError());
            exit_code = 1;
            break;
        }
        if(event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            break;
        }
    }

    return exit_code;
}

/** @brief Displays test canvas pixels until the window closes. Returns nonzero on failure. */
int main(int num_args, char** args) {
    (void)num_args; // Ignored.
    (void)args;     // Ignored.

    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Could not initialize SDL: %s", SDL_GetError());
        return 1;
    }

    const int exit_code = run_app();
    SDL_Quit();
    return exit_code;
}
