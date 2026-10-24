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
    if (x < 0 || y < 0 || static_cast<unsigned int>(x) >= _pixel_width
        || static_cast<unsigned int>(y) >= _pixel_height) {
        return;
    }

    const auto red = static_cast<std::uint32_t>(c.red) & 0xFFu;
    const auto green = static_cast<std::uint32_t>(c.green) & 0xFFu;
    const auto blue = static_cast<std::uint32_t>(c.blue) & 0xFFu;
    const auto alpha = static_cast<std::uint32_t>(c.alpha) & 0xFFu;
    const auto pixel_index = static_cast<std::size_t>(y) * _pixel_width + static_cast<std::size_t>(x);
    _pixels[pixel_index] = (red << 24) | (green << 16) | (blue << 8) | alpha;
}

