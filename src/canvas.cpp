module;

#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>

module canvas;

Canvas::Canvas(unsigned int pixel_width, unsigned int pixel_height)
    : _pixel_width(pixel_width), _pixel_height(pixel_height) {
    const auto width = static_cast<std::size_t>(pixel_width);
    const auto height = static_cast<std::size_t>(pixel_height);
    if (height != 0 && width > std::numeric_limits<std::size_t>::max() / sizeof(std::uint32_t) / height) {
        throw std::bad_array_new_length{};
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

