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

void draw_checker(Canvas* canvas) {
    Color grid_color = colors::white();
    grid_color.alpha(64);
    for(int x = 0; x < canvas->get_width(); ++x) {
        for(int y = 0; y < canvas->get_height(); ++y) {
            if((x + y) % 2 == 0) {
                canvas->set_pixel(x, y, grid_color);
            }
        }
    }
}

// I can't find my old Bresenham line drawing code from 20 years ago, so I based this off of the YouTube video lesson by NoBS Code.
// Video can be seen here: https://youtu.be/CceepU1vIKo
void _draw_line_horizontal(Canvas* canvas, std::int32_t start_x, std::int32_t start_y, std::int32_t end_x, std::int32_t end_y) {
    if(start_x > end_x) {
        std::swap(start_x, end_x);
        std::swap(start_y, end_y);
    }

    std::int32_t delta_x = end_x - start_x;
    std::int32_t delta_y = end_y - start_y;
    std::int32_t direction = delta_y >= 0 ? 1 : -1;

    delta_y *= direction;

    if(delta_x != 0) {
        std::int32_t y = start_y;
        std::int32_t decider = 2 * delta_y - delta_x;
        for(auto x = 0; x <= delta_x; ++x) {
            canvas->set_pixel(start_x + x, y, colors::lime());
            if(decider >= 0) {
                y += direction;
                decider -= 2 * delta_x;
            }
            decider += 2 * delta_y;
        }
    }
}

void _draw_line_vertical(Canvas* canvas, std::int32_t start_x, std::int32_t start_y, std::int32_t end_x, std::int32_t end_y) {
    if(start_y > end_y) {
        std::swap(start_x, end_x);
        std::swap(start_y, end_y);
    }

    std::int32_t delta_x = end_x - start_x;
    std::int32_t delta_y = end_y - start_y;
    std::int32_t direction = delta_x >= 0 ? 1 : -1;

    delta_x *= direction;
    if(delta_y != 0) {
        std::int32_t x = start_x;
        std::int32_t decider = 2 * delta_x - delta_y;
        for(auto y = 0; y <= delta_y; ++y) {
            canvas->set_pixel(x, start_y + y, colors::light_blue());
            if(decider >= 0) {
                x += direction;
                decider -= 2 * delta_y;
            }
            decider += 2 * delta_x;
        }
    }
}

void bresenham_line(Canvas* canvas, std::int32_t start_x, std::int32_t start_y, std::int32_t end_x, std::int32_t end_y) {
    if(std::abs(end_x - start_x) > std::abs(end_y - start_y)) {
        _draw_line_horizontal(canvas, start_x, start_y, end_x, end_y);
    }
    else {
        _draw_line_vertical(canvas, start_x, start_y, end_x, end_y);
    }
}

void _draw_antialias_line_horizontal(Canvas* canvas, double start_x, double start_y, double end_x, double end_y) {
    Color pixel_color = colors::lime();
    if(end_x < start_x) {
        std::swap(start_x, end_x);
        std::swap(start_y, end_y);
    }
    double delta_x = end_x - start_x;
    double delta_y = end_y - start_y;
    double slope = delta_x != 0 ? delta_y / delta_x : 1;

    // Draw the start pixel.
    double endpoint_overlap = 1 - ((start_x + 0.5) - std::floor(start_x + 0.5));
    double subpixel_offset = start_y - std::floor(start_y);
    pixel_color.alpha((1 - subpixel_offset) * endpoint_overlap);
    canvas->set_pixel(int(std::floor(start_x + 0.5)), int(std::floor(start_y)), pixel_color);
    pixel_color.alpha(subpixel_offset * endpoint_overlap);
    canvas->set_pixel(int(std::floor(start_x + 0.5)), int(std::floor(start_y) + 1), pixel_color);

    // Draw the end pixel.
    endpoint_overlap = ((end_x - 0.5) - std::floor(end_x - 0.5));
    subpixel_offset = end_y - std::floor(end_y);
    pixel_color.alpha((1 - subpixel_offset) * endpoint_overlap);
    canvas->set_pixel(int(std::floor(end_x + 0.5)), int(std::floor(end_y)), pixel_color);
    pixel_color.alpha(subpixel_offset * endpoint_overlap);
    canvas->set_pixel(int(std::floor(end_x + 0.5)), int(std::floor(start_y) + 1), pixel_color);

    // Draw remaining inbetween pixels.
    for(auto i = 1; i < std::floor(delta_x); ++i) {
        double x = start_x + i;
        double y = start_y + i * slope;
        std::int32_t ix = std::floor(x);
        std::int32_t iy = std::floor(y);
        double dist_from_pixel = y - iy;

        // pixel_color = colors::red();
        pixel_color.alpha(1 - dist_from_pixel);
        canvas->set_pixel(ix, iy, pixel_color);

        // pixel_color = colors::orange();
        pixel_color.alpha(dist_from_pixel);
        canvas->set_pixel(ix, iy + 1, pixel_color);
    }
}

void _draw_antialias_line_vertical(Canvas* canvas, double start_x, double start_y, double end_x, double end_y) {
    Color pixel_color = colors::lime();
    if(end_y < start_y) {
        std::swap(start_x, end_x);
        std::swap(start_y, end_y);
    }
    double delta_x = end_x - start_x;
    double delta_y = end_y - start_y;
    double slope = delta_y != 0 ? delta_x / delta_y : 1;

    // Draw the start pixel.
    double endpoint_overlap = 1 - ((start_y + 0.5) - std::floor(start_y + 0.5));
    double subpixel_offset = start_y - std::floor(start_y);
    pixel_color.alpha((1 - subpixel_offset) * endpoint_overlap);
    canvas->set_pixel(int(std::floor(start_x + 0.5)), int(std::floor(start_y)), pixel_color);
    pixel_color.alpha(subpixel_offset * endpoint_overlap);
    canvas->set_pixel(int(std::floor(start_x + 0.5)), int(std::floor(start_y) + 1), pixel_color);

    // Draw the end pixel.
    endpoint_overlap = ((end_y - 0.5) - std::floor(end_y - 0.5));
    subpixel_offset = end_y - std::floor(end_y);
    pixel_color.alpha((1 - subpixel_offset) * endpoint_overlap);
    canvas->set_pixel(int(std::floor(end_x + 0.5)), int(std::floor(end_y)), pixel_color);
    pixel_color.alpha(subpixel_offset * endpoint_overlap);
    canvas->set_pixel(int(std::floor(end_x + 0.5)), int(std::floor(start_y) + 1), pixel_color);

    for(auto i = 1; i < int(delta_y); ++i) {
        double x = start_x + i * slope;
        double y = start_y + i;
        std::int32_t ix = std::floor(x);
        std::int32_t iy = std::floor(y);
        double dist_from_pixel = x - ix;

        // pixel_color = colors::yellow();
        pixel_color.alpha(1 - dist_from_pixel);
        canvas->set_pixel(ix, iy, pixel_color);

        // pixel_color = colors::cyan();
        pixel_color.alpha(dist_from_pixel);
        canvas->set_pixel(ix + 1, iy, pixel_color);
    }
}

void xiaolin_wu_line(Canvas* canvas, const double start_x, const double start_y, const double end_x, const double end_y) {
    if(std::abs(end_y - start_y) < std::abs(end_x - start_x)) {
        _draw_antialias_line_horizontal(canvas, start_x, start_y, end_x, end_y);
    } else {
        _draw_antialias_line_vertical(canvas, start_x, start_y, end_x, end_y);
    }
}

void draw_circle(Canvas* canvas, std::int32_t center_x, std::int32_t center_y, std::int32_t radius) {
    Color quadrant_1 = colors::red();
    Color quadrant_2 = colors::green();
    Color quadrant_3 = colors::blue();
    Color quadrant_4 = colors::cyan();
    Color quadrant_5 = colors::yellow();
    Color quadrant_6 = colors::magenta();
    Color quadrant_7 = colors::white();
    Color quadrant_8 = colors::orange();

    quadrant_1.alpha(0.25);
    quadrant_2.alpha(0.25);
    quadrant_3.alpha(0.25);
    quadrant_4.alpha(0.25);
    quadrant_5.alpha(0.25);
    quadrant_6.alpha(0.25);
    quadrant_7.alpha(0.25);
    quadrant_8.alpha(0.25);

    // BUG: Initial pixel draw causes overdraw.
    // BUG: When there's a diagonal pixel, overdraw occurs.
    std::int32_t x = 0;
    std::int32_t y = -radius;
    std::int32_t decider = -radius;


    while(x < -y) {
        if(decider > 0) {
            y += 1;
            decider += 2 * (x + y) + 1;
        }
        else {
            decider += 2 * x + 1;
        }

        canvas->set_pixel(center_x + x, center_y + y, quadrant_1); canvas->show(); // Bottom (moving right) [lower-right diagonal]
        canvas->set_pixel(center_x + x, center_y - y, quadrant_2); canvas->show(); // Top (moving right) [upper-right diagonal]
        canvas->set_pixel(center_x - x, center_y + y, quadrant_3); canvas->show(); // Bottom, again (moving left) [lower-left diagonal]
        canvas->set_pixel(center_x - x, center_y - y, quadrant_4); canvas->show(); // Top, again (moving left) [upper-left diagonal]
        canvas->set_pixel(center_x + y, center_y + x, quadrant_5); canvas->show(); // Left (moving up) [UL again, diagonal]
        canvas->set_pixel(center_x + y, center_y - x, quadrant_6); canvas->show(); // Left, again (moving down) [LL again, diagonal]
        canvas->set_pixel(center_x - y, center_y + x, quadrant_7); canvas->show(); // Right (moving up) [UR again, diagonal]
        canvas->set_pixel(center_x - y, center_y - x, quadrant_8); canvas->show(); // Right, again (moving down) [LR again, diagonal]
        x += 1;
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
    // draw_checker(canvas.get());
    canvas->use_centered_coordinates();
    canvas->set_pixel(0, 0, colors::red());

    //bresenham_line(canvas.get(), 5,0, 11,0);      // horizontal straight right
    //bresenham_line(canvas.get(), 2, 1, 13, 4);    // shallow upper right
    //bresenham_line(canvas.get(), 5,5, 11,11);     // diagonal upper right
    //bresenham_line(canvas.get(), 1,2, 4,13);      // steep upper right
    //bresenham_line(canvas.get(), 0,5, 0,11);      // vertical up
    //bresenham_line(canvas.get(), -1,2, -4,13);    // steep upper left
    //bresenham_line(canvas.get(), -5,5, -11,11);   // diagonal upper left
    //bresenham_line(canvas.get(), -2,1, -13,4);    // shallow upper left
    //bresenham_line(canvas.get(), -5,0, -11,0);    // horizontal straight left
    //bresenham_line(canvas.get(), -2,-1, -13,-4);  // shallow lower left
    //bresenham_line(canvas.get(), -5,-5, -11,-11); // diagonal lower left
    //bresenham_line(canvas.get(), -1,-2, -4,-13);  // steep lower left
    //bresenham_line(canvas.get(), 0,-5, 0,-11);    // vertical down
    //bresenham_line(canvas.get(), 1,-2, 4,-13);    // steep lower right
    //bresenham_line(canvas.get(), 5,-5, 11,-11);   // diagonal lower right
    //bresenham_line(canvas.get(), 2, -1, 13, -4);  // shallow lower right

    //xiaolin_wu_line(canvas.get(), 5,0, 11,0);      // horizontal straight right
    //xiaolin_wu_line(canvas.get(), 2, 1, 13, 4);    // shallow upper right
    //xiaolin_wu_line(canvas.get(), 5,5, 11,11);     // diagonal upper right
    //xiaolin_wu_line(canvas.get(), 1,2, 4,13);      // steep upper right
    //xiaolin_wu_line(canvas.get(), 0,5, 0,11);      // vertical up
    //xiaolin_wu_line(canvas.get(), -1,2, -4,13);    // steep upper left
    //xiaolin_wu_line(canvas.get(), -5,5, -11,11);   // diagonal upper left
    //xiaolin_wu_line(canvas.get(), -2,1, -13,4);    // shallow upper left
    //xiaolin_wu_line(canvas.get(), -5,0, -11,0);    // horizontal straight left
    //xiaolin_wu_line(canvas.get(), -2,-1, -13,-4);  // shallow lower left
    //xiaolin_wu_line(canvas.get(), -5,-5, -11,-11); // diagonal lower left
    //xiaolin_wu_line(canvas.get(), -1,-2, -4,-13);  // steep lower left
    //xiaolin_wu_line(canvas.get(), 0,-5, 0,-11);    // vertical down
    //xiaolin_wu_line(canvas.get(), 1,-2, 4,-13);    // steep lower right
    //xiaolin_wu_line(canvas.get(), 5,-5, 11,-11);   // diagonal lower right
    //xiaolin_wu_line(canvas.get(), 2, -1, 13, -4);  // shallow lower right

    // TEMPORARY: Keyboard-controlled radius for experimenting with circle drawing.
    std::int32_t radius = 10;
    bool redraw_circle = true;

    // Core App Loop
    int exit_code = 0;
    while(true) {
        // TEMPORARY: Erase the previous circle before drawing the updated radius.
        if(redraw_circle) {
            canvas->use_centered_coordinates(false);
            for(int y = 0; y < canvas_height; ++y) {
                for(int x = 0; x < canvas_width; ++x) {
                    canvas->set_pixel(x, y, colors::black());
                }
            }
            canvas->use_centered_coordinates();
            canvas->set_pixel(0, 0, colors::red());
            draw_circle(canvas.get(), 0, 0, radius);
            redraw_circle = false;
        }

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
        // TEMPORARY: Adjust once per key press; keep the radius nonnegative.
        if(event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
            if(event.key.key == SDLK_UP) {
                ++radius;
                redraw_circle = true;
            }
            else if(event.key.key == SDLK_DOWN && radius > 0) {
                --radius;
                redraw_circle = true;
            }
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
