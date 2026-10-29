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

/**
 * @brief TEMPORARY: Draws a translucent reference circle by comparing radial distances.
 * @note Visual test code for draw_circle(); its long-term API and retention are undecided.
 * @param canvas Destination canvas; must not be null.
 * @param center_x Horizontal center in the canvas coordinate mode.
 * @param center_y Vertical center in the canvas coordinate mode.
 * @param radius Positive radius in pixels; zero and negative radii are not handled separately.
 */
void draw_perfect_circle(Canvas* canvas, int center_x, int center_y, int radius) {
    Color color{80,224,152,52};
    int x = 0;
    int y = radius;
    canvas->set_pixel(center_x, center_y + radius, color);
    canvas->set_pixel(center_x, center_y - radius, color);
    canvas->set_pixel(center_x + radius, center_y, color);
    canvas->set_pixel(center_x - radius, center_y, color);

    while (true) {
        x += 1;
        if(std::abs(std::hypot(static_cast<double>(x), static_cast<double>(y) - 1) - radius)
           < std::abs(std::hypot(static_cast<double>(x), static_cast<double>(y)) - radius)) {
            --y;
        }
        if (x < y) {
            canvas->set_pixel(center_x + x, center_y + y, color);
            canvas->set_pixel(center_x - x, center_y + y, color);
            canvas->set_pixel(center_x + x, center_y - y, color);
            canvas->set_pixel(center_x - x, center_y - y, color);
            canvas->set_pixel(center_x + y, center_y + x, color);
            canvas->set_pixel(center_x - y, center_y + x, color);
            canvas->set_pixel(center_x + y, center_y - x, color);
            canvas->set_pixel(center_x - y, center_y - x, color);
        }
        else
            {break;}
    }
    if (x == y) {
        canvas->set_pixel(center_x + x, center_y + y, color);
        canvas->set_pixel(center_x - x, center_y + y, color);
        canvas->set_pixel(center_x + x, center_y - y, color);
        canvas->set_pixel(center_x - x, center_y - y, color);
    }
}

void draw_circle(Canvas* canvas, std::int32_t center_x, std::int32_t center_y, std::int32_t radius) {
    if(radius < 1) { canvas->set_pixel(center_x, center_y, colors::red()); }

    Color quadrant_1 = {240, 80, 160, 52}; //colors::red();
    Color quadrant_2 = {240, 80, 160, 52}; //colors::green();
    Color quadrant_3 = {240, 80, 160, 52}; //colors::blue();
    Color quadrant_4 = {240, 80, 160, 52}; //colors::cyan();
    Color quadrant_5 = {240, 80, 160, 52}; //colors::yellow();
    Color quadrant_6 = {240, 80, 160, 52}; //colors::magenta();
    Color quadrant_7 = {240, 80, 160, 52}; //colors::white();
    Color quadrant_8 = {240, 80, 160, 52}; //colors::orange();

    //quadrant_1.alpha(0.10);
    //quadrant_2.alpha(0.10);
    //quadrant_3.alpha(0.10);
    //quadrant_4.alpha(0.10);
    //quadrant_5.alpha(0.10);
    //quadrant_6.alpha(0.10);
    //quadrant_7.alpha(0.10);
    //quadrant_8.alpha(0.10);

    // BUG: Initial pixel draw causes overdraw.
    // BUG: When there's a diagonal pixel, overdraw occurs.
    std::int32_t x = 0;
    std::int32_t y = -radius;
    std::int32_t decider = 1 - radius;

    // Plot compass directions to prevent overdraw.
    canvas->set_pixel(center_x + x, center_y + y, quadrant_1);
    canvas->set_pixel(center_x - x, center_y - y, quadrant_4);
    canvas->set_pixel(center_x + y, center_y - x, quadrant_6);
    canvas->set_pixel(center_x - y, center_y + x, quadrant_7);

    while(x < -y) {
        x += 1;
        if(decider >= 0) {
            y += 1;
            decider += 2 * (x + y) + 1;
        }
        else {
            decider += 2 * x + 1;
        }

        if(x < -y) {
            canvas->set_pixel(center_x + x, center_y + y, quadrant_1); // canvas->show(); // Bottom (moving right) [lower-right diagonal]
            canvas->set_pixel(center_x + x, center_y - y, quadrant_2); // canvas->show(); // Top (moving right) [upper-right diagonal]
            canvas->set_pixel(center_x - x, center_y + y, quadrant_3); // canvas->show(); // Bottom, again (moving left) [lower-left diagonal]
            canvas->set_pixel(center_x - x, center_y - y, quadrant_4); // canvas->show(); // Top, again (moving left) [upper-left diagonal]
            canvas->set_pixel(center_x + y, center_y + x, quadrant_5); // canvas->show(); // Left (moving up) [UL again, diagonal]
            canvas->set_pixel(center_x + y, center_y - x, quadrant_6); // canvas->show(); // Left, again (moving down) [LL again, diagonal]
            canvas->set_pixel(center_x - y, center_y + x, quadrant_7); // canvas->show(); // Right (moving up) [UR again, diagonal]
            canvas->set_pixel(center_x - y, center_y - x, quadrant_8); // canvas->show(); // Right, again (moving down) [LR again, diagonal]
        }
        else
        {
            break;
        }
    }
    if (x == -y){
        canvas->set_pixel(center_x + x, center_y + y, quadrant_1);
        canvas->set_pixel(center_x + x, center_y - y, quadrant_2);
        canvas->set_pixel(center_x - x, center_y + y, quadrant_3);
        canvas->set_pixel(center_x - x, center_y - y, quadrant_4);
    }

}


/**
 * @brief Creates display resources and runs the temporary circle comparison harness.
 * @note SDL must be initialized. The demo drawing, colors, sizing, and keyboard controls
 *       are temporary visual test code for the circle algorithms, not application APIs.
 */
static int run_app() {
    // Data
    int window_width = 1600;
    int window_height = 900;
    unsigned int canvas_width = 16 * 8;
    unsigned int canvas_height = 9 * 8;

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
    constexpr Uint64 RADIUS_UPDATE_INTERVAL_MS = 50;
    bool up_pressed = false;
    bool down_pressed = false;
    Uint64 next_radius_update_ms = 0;

    // Core App Loop
    int exit_code = 0;
    while(true) {
        // TEMPORARY: Repeat held arrows at 20 updates per second.
        const int radius_direction = int(up_pressed) - int(down_pressed);
        const Uint64 current_time_ms = SDL_GetTicks();
        if(radius_direction != 0 && current_time_ms >= next_radius_update_ms) {
            if(radius_direction > 0 || radius > 0) {
                radius += radius_direction;
                redraw_circle = true;
            }
            next_radius_update_ms = current_time_ms + RADIUS_UPDATE_INTERVAL_MS;
        }

        // TEMPORARY: Erase the previous circle before drawing the updated radius.
        if(redraw_circle) {
            canvas->use_centered_coordinates(false);
            for(int y = 0; y < canvas_height; ++y) {
                for(int x = 0; x < canvas_width; ++x) {
                    canvas->set_pixel(x, y, colors::black());
                }
            }
            canvas->use_centered_coordinates();
            // canvas->set_pixel(0, 0, colors::red());
            draw_circle(canvas.get(), 0, 0, radius);
            draw_perfect_circle(canvas.get(), 0, 0, radius);
            redraw_circle = false;
        }

        if(!canvas->show()) {
            SDL_Log("Could not show canvas: %s", SDL_GetError());
            exit_code = 1;
            break;
        }

        // TEMPORARY: Wake for the next radius update even when no events arrive.
        const Uint64 wait_start_ms = SDL_GetTicks();
        const int wait_timeout_ms = radius_direction == 0 ? -1
            : next_radius_update_ms > wait_start_ms ? int(next_radius_update_ms - wait_start_ms) : 0;
        SDL_Event event;
        SDL_ClearError();
        if(!SDL_WaitEventTimeout(&event, wait_timeout_ms)) {
            if(*SDL_GetError() != '\0') {
                SDL_Log("Could not wait for an event: %s", SDL_GetError());
                exit_code = 1;
                break;
            }
            continue;
        }
        if(event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            break;
        }
        // TEMPORARY: Track held arrows independently of operating-system key repeat.
        if(event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
            if(event.key.key == SDLK_UP) {
                up_pressed = true;
                next_radius_update_ms = SDL_GetTicks();
            }
            else if(event.key.key == SDLK_DOWN) {
                down_pressed = true;
                next_radius_update_ms = SDL_GetTicks();
            }
            // TEMPORARY: Left/right fine tuning changes the radius once per press.
            else if(event.key.key == SDLK_RIGHT) {
                ++radius;
                redraw_circle = true;
            }
            else if(event.key.key == SDLK_LEFT && radius > 0) {
                --radius;
                redraw_circle = true;
            }
        }
        else if(event.type == SDL_EVENT_KEY_UP) {
            if(event.key.key == SDLK_UP) { up_pressed = false; }
            else if(event.key.key == SDLK_DOWN) { down_pressed = false; }
        }
        else if(event.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
            up_pressed = false;
            down_pressed = false;
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
