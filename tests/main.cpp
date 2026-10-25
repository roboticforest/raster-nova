#include <SDL3/SDL.h>
#include <iostream>
#include <limits>
#include <memory>

import canvas;

/** @brief Reports a failed check and returns 1 on failure, or 0 on success. */
static int check(bool passed, const char* description) {
    if (passed) {
        return 0;
    }
    std::cerr << "FAIL: " << description << '\n';
    return 1;
}

/** @brief Compares a stored pixel with an expected color and returns 1 on failure. */
static int check_pixel(const Canvas& canvas, int x, int y,
                       Color expected, const char* description) {
    const Color actual = canvas.get_pixel(x, y);
    if (actual.rgba() == expected.rgba()) {
        return 0;
    }
    std::cerr << "FAIL: " << description << " at (" << x << ", " << y
              << "): expected RGBA 0x" << std::hex << expected.rgba()
              << ", got 0x" << actual.rgba() << std::dec << '\n';
    return 1;
}

/** @brief Runs Canvas checks without a window; returns nonzero if any check fails. */
int main() {
    // The surface must outlive its software renderer. No video initialization is needed.
    const std::shared_ptr<SDL_Surface> surface{
        SDL_CreateSurface(3, 2, SDL_PIXELFORMAT_RGBA8888), SDL_DestroySurface};
    if (!surface) {
        std::cerr << "Could not create test surface: " << SDL_GetError() << '\n';
        return 1;
    }
    const std::shared_ptr<SDL_Renderer> renderer{
        SDL_CreateSoftwareRenderer(surface.get()), [surface](SDL_Renderer* renderer) {
            SDL_DestroyRenderer(renderer);
        }};
    if (!renderer) {
        std::cerr << "Could not create test renderer: " << SDL_GetError() << '\n';
        return 1;
    }

    int num_failures = 0;

    std::cout << "Initial pixels and corner drawing\n";
    {
        Canvas canvas{renderer, 3, 2};
        for (unsigned int y = 0; y < 2; ++y) {
            for (unsigned int x = 0; x < 3; ++x) {
                num_failures += check_pixel(canvas, x, y, {}, "New pixels are transparent black");
            }
        }

        canvas.set_pixel(0, 0, colors::red());
        canvas.set_pixel(2, 1, colors::blue());
        for (unsigned int y = 0; y < 2; ++y) {
            for (unsigned int x = 0; x < 3; ++x) {
                Color expected;
                if (x == 0 && y == 0) {
                    expected = colors::red();
                } else if (x == 2 && y == 1) {
                    expected = colors::blue();
                }
                num_failures += check_pixel(canvas, x, y, expected, "Only the two corners change");
            }
        }
        num_failures += check(canvas.show(), "Presenting through the software renderer succeeds");
    }

    std::cout << "Zero-size canvas\n";
    {
        Canvas canvas{renderer, 0, 0};
        canvas.set_pixel(0, 0, colors::red());
        num_failures += check_pixel(canvas, 0, 0, {}, "Drawing on an empty canvas stores no pixel");
        num_failures += check(canvas.show(), "Showing an empty canvas succeeds");
    }

    std::cout << "Out-of-bounds pixels\n";
    {
        Canvas canvas{renderer, 3, 2};
        canvas.set_pixel(1, 0, colors::lime());
        canvas.set_pixel(-1, 0, colors::red());
        canvas.set_pixel(3, 0, colors::red());
        canvas.set_pixel(0, -1, colors::red());
        canvas.set_pixel(0, 2, colors::red());
        for (unsigned int y = 0; y < 2; ++y) {
            for (unsigned int x = 0; x < 3; ++x) {
                const Color expected = x == 1 && y == 0 ? colors::lime() : Color{};
                num_failures += check_pixel(canvas, x, y, expected, "Out-of-bounds drawing changes no pixels");
            }
        }
        num_failures += check_pixel(canvas, 3, 0, {}, "Reading past the right edge returns transparent black");
        num_failures += check_pixel(canvas, 0, 2, {}, "Reading past the bottom edge returns transparent black");
    }

    std::cout << "Alpha blending\n";
    {
        Canvas canvas{renderer, 3, 2};
        canvas.set_pixel(1, 0, {0, 0, 255, 127});
        num_failures += check_pixel(canvas, 1, 0, {0, 0, 255, 127}, "Blue over transparent black");
        canvas.set_pixel(1, 0, {255, 0, 0, 127});
        num_failures += check_pixel(canvas, 1, 0, {170, 0, 85, 191}, "Red over translucent blue");
        canvas.set_pixel(2, 0, {255, 0, 0, 127});
        num_failures += check_pixel(canvas, 2, 0, {255, 0, 0, 127}, "Red over transparent black");
        canvas.set_pixel(2, 0, {0, 0, 255, 127});
        num_failures += check_pixel(canvas, 2, 0, {85, 0, 170, 191}, "Blue over translucent red");
    }

    std::cout << "Centered and normalized coordinates\n";
    {
        Canvas canvas{renderer, 3, 2};
        canvas.use_centered_coordinates();
        canvas.set_pixel(0, 0, colors::white());
        num_failures += check_pixel(canvas, 0, 0, colors::white(), "Centered integer reads should match writes");
        canvas.use_centered_coordinates(false);
        num_failures += check_pixel(canvas, 1, 0, colors::white(), "Centered integer origin");
        canvas.use_centered_coordinates();
        canvas.set_pixel(0.0, 0.0, colors::orange());
        num_failures += check(canvas.get_pixel(0.0, 0.0).rgba() == colors::orange().rgba(),
                              "Normalized reads should match writes");
        canvas.use_centered_coordinates(false);
        num_failures += check_pixel(canvas, 1, 0, colors::orange(), "Normalized origin matches integer origin");
        canvas.use_centered_coordinates();
        canvas.set_pixel(-1.0, -1.0, colors::lime());
        canvas.set_pixel(1.0, 1.0, colors::red());
        canvas.use_centered_coordinates(false);
        num_failures += check_pixel(canvas, 0, 1, colors::lime(), "Normalized bottom-left endpoint");
        num_failures += check_pixel(canvas, 2, 0, colors::red(), "Normalized top-right endpoint");
    }

    std::cout << "Coordinate conversions and mirrored pixel access\n";
    for(unsigned int width = 0; width <= 4; ++width) {
        for(unsigned int height = 0; height <= 4; ++height) {
            Canvas canvas{renderer, width, height};
            for(const bool centered : {false, true}) {
                canvas.use_centered_coordinates(centered);
                num_failures += check(!canvas.to_normalized_coordinates(width, 0)
                                      && !canvas.to_normalized_coordinates(0, height),
                                      "Raw coordinates outside the canvas should not convert");
                if(width == 0 || height == 0) {
                    num_failures += check(!canvas.to_raw_pixels(0, 0) && !canvas.to_raw_pixels(0.0, 0.0),
                                          "Empty canvases should have no pixel coordinates");
                    num_failures += check(canvas.get_pixel(0.0, 0.0).rgba() == Color{}.rgba(),
                                          "Normalized reads on empty canvases should return transparent black");
                }
                for(unsigned int y = 0; y < height; ++y) {
                    for(unsigned int x = 0; x < width; ++x) {
                        const int logical_x = centered ? static_cast<int>(x) - static_cast<int>(width / 2) : static_cast<int>(x);
                        const int logical_y = centered ? static_cast<int>((height - 1) / 2) - static_cast<int>(y) : static_cast<int>(y);
                        const auto integer_pixel = canvas.to_raw_pixels(logical_x, logical_y);
                        num_failures += check(integer_pixel && integer_pixel->x == x && integer_pixel->y == y,
                                              "Integer coordinates should convert to the expected raw pixel");
                        canvas.set_pixel(logical_x, logical_y, colors::blue());
                        num_failures += check_pixel(canvas, logical_x, logical_y, colors::blue(),
                                                    "Integer reads and writes should select the same pixel");
                        const auto normalized = canvas.to_normalized_coordinates(x, y);
                        num_failures += check(normalized.has_value(), "Every valid pixel should convert to normalized coordinates");
                        if(!normalized) { continue; }
                        const auto pixel = canvas.to_raw_pixels(normalized->x, normalized->y);
                        num_failures += check(pixel && pixel->x == x && pixel->y == y,
                                              "Normalized coordinates should round-trip to the original pixel");
                        canvas.set_pixel(normalized->x, normalized->y, colors::orange());
                        num_failures += check(canvas.get_pixel(normalized->x, normalized->y).rgba() == colors::orange().rgba(),
                                              "Normalized reads and writes should select the same pixel");
                        canvas.use_centered_coordinates(false);
                        num_failures += check_pixel(canvas, x, y, colors::orange(), "Converted coordinates should select the expected raw pixel");
                        canvas.use_centered_coordinates(centered);
                    }
                }
            }
        }
    }

    {
        Canvas canvas{renderer, 4, 4};
        for(const bool centered : {false, true}) {
            canvas.use_centered_coordinates(centered);
            const auto pixel = canvas.to_raw_pixels(0.25, 0.75);
            num_failures += check(pixel && pixel->x == (centered ? 2u : 1u) && pixel->y == (centered ? 0u : 2u),
                                  "Fractional coordinates should retain the existing rounding and axis directions");
            canvas.set_pixel(0.25, 0.75, colors::lime());
            for(const double invalid : {-2.0, 2.0, std::numeric_limits<double>::infinity(),
                                        std::numeric_limits<double>::quiet_NaN()}) {
                num_failures += check(!canvas.to_raw_pixels(invalid, 0.0) && !canvas.to_raw_pixels(0.0, invalid),
                                      "Invalid normalized coordinates should not convert");
                canvas.set_pixel(invalid, 0.0, colors::red());
                canvas.set_pixel(0.0, invalid, colors::red());
                num_failures += check(canvas.get_pixel(invalid, 0.0).rgba() == Color{}.rgba()
                                      && canvas.get_pixel(0.0, invalid).rgba() == Color{}.rgba(),
                                      "Invalid normalized reads should return transparent black");
            }
            num_failures += check(canvas.get_pixel(0.25, 0.75).rgba() == colors::lime().rgba(),
                                  "Invalid normalized writes should leave existing pixels unchanged");
        }
    }

    // Add new cases here using a fresh Canvas when they need an empty buffer.

    std::cout << num_failures << " failed check(s).\n";
    return num_failures == 0 ? 0 : 1;
}
