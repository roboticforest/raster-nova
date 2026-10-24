module;

#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>

module canvas;

Canvas::Canvas(int pixel_width, int pixel_height)
    : _pixel_width(pixel_width), _pixel_height(pixel_height) {
    if (pixel_width <= 0 || pixel_height <= 0) {
        throw std::invalid_argument("Canvas dimensions must be positive.");
    }

    const auto width = static_cast<std::size_t>(pixel_width);
    const auto height = static_cast<std::size_t>(pixel_height);
    if (width > std::numeric_limits<std::size_t>::max() / sizeof(std::uint32_t) / height) {
        throw std::length_error("Canvas pixel buffer is too large.");
    }

    // Each pixel represents 0xRRGGBBAA; its byte order depends on the platform.
    _pixels = new std::uint32_t[width * height]{};
}

Canvas::~Canvas() {
    delete[] _pixels;
}

void Canvas::set_pixel(int x, int y, Color c) {
    // Pixel storage and rendering will be implemented later.
    (void)x;
    (void)y;
    (void)c;
}

