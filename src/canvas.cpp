module;

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <limits>
#include <new>
#include <utility>
#include <SDL3/SDL.h>

module canvas;

Canvas::Canvas(std::shared_ptr<SDL_Renderer> renderer, unsigned int pixel_width, unsigned int pixel_height)
    : _renderer(std::move(renderer)), _pixel_width(pixel_width), _pixel_height(pixel_height) {
    const auto width = static_cast<std::size_t>(pixel_width);
    const auto height = static_cast<std::size_t>(pixel_height);
    if (height != 0 && width > std::numeric_limits<std::size_t>::max() / sizeof(std::uint32_t) / height) {
        throw std::bad_array_new_length{};
    }

    // Each pixel represents 0xRRGGBBAA; its byte order depends on the platform.
    _pixels = new std::uint32_t[width * height]{};
}

Canvas::~Canvas() {
    SDL_DestroyTexture(_texture);
    delete[] _pixels;
}

void Canvas::set_pixel(int x, int y, Color c) {
    _set_raw_pixel(to_raw_pixels(x, y), c);
}

PixelCoordinates Canvas::to_raw_pixels(int x, int y) const {
    std::int64_t screen_x = x;
    std::int64_t screen_y = y;
    if (_centered_coordinates) {
        screen_x = static_cast<std::int64_t>(_pixel_width / 2) + x;
        screen_y = (static_cast<std::int64_t>(_pixel_height) - 1) / 2 - y;
    }
    return {screen_x, screen_y};
}

void Canvas::use_centered_coordinates(bool enabled) {
    _centered_coordinates = enabled;
}

Color Canvas::get_pixel(int x, int y) const {
    const auto pixel = to_raw_pixels(x, y);
    if (!_contains_pixel(pixel)) {
        return {};
    }
    return Color::from_rgba(_pixels[static_cast<std::size_t>(pixel.y) * _pixel_width + pixel.x]);
}

Color Canvas::get_pixel(double x, double y) const {
    if (!_contains_normalized_coordinates(x, y)) {
        return {};
    }
    const auto pixel = to_raw_pixels(x, y);
    return Color::from_rgba(_pixels[static_cast<std::size_t>(pixel.y) * _pixel_width + pixel.x]);
}

void Canvas::set_pixel(double x, double y, Color c) {
    if (_contains_normalized_coordinates(x, y)) {
        _set_raw_pixel(to_raw_pixels(x, y), c);
    }
}

PixelCoordinates Canvas::to_raw_pixels(double x, double y) const {
    if (_centered_coordinates) {
        x = (x + 1.0) / 2.0;
        y = (y + 1.0) / 2.0;
    }
    const auto horizontal_span = _pixel_width > 0 ? _pixel_width - 1 : 0;
    const auto vertical_span = _pixel_height > 0 ? _pixel_height - 1 : 0;
    const auto pixel_x = static_cast<std::int64_t>(std::round(x * horizontal_span));
    const double rounded_y = std::round(y * vertical_span);
    const auto pixel_y = static_cast<std::int64_t>(_centered_coordinates ? vertical_span - rounded_y : rounded_y);
    return {pixel_x, pixel_y};
}

NormalizedCoordinates Canvas::to_normalized_coordinates(std::int64_t x, std::int64_t y) const {
    double normalized_x = _pixel_width > 1 ? static_cast<double>(x) / (_pixel_width - 1) : 0.0;
    double normalized_y = _pixel_height > 1 ? static_cast<double>(y) / (_pixel_height - 1) : 0.0;
    if (_centered_coordinates) {
        normalized_x = _pixel_width > 1 ? 2.0 * normalized_x - 1.0 : 0.0;
        normalized_y = _pixel_height > 1 ? 1.0 - 2.0 * normalized_y : 0.0;
    }
    return {normalized_x, normalized_y};
}

bool Canvas::_contains_pixel(PixelCoordinates pixel) const {
    return pixel.x >= 0 && pixel.y >= 0 && pixel.x < _pixel_width && pixel.y < _pixel_height;
}

bool Canvas::_contains_normalized_coordinates(double x, double y) const {
    const double coordinate_min = _centered_coordinates ? -1.0 : 0.0;
    return x >= coordinate_min && x <= 1.0 && y >= coordinate_min && y <= 1.0
        && _pixel_width != 0 && _pixel_height != 0;
}

void Canvas::_set_raw_pixel(PixelCoordinates pixel, Color c) {
    if (!_contains_pixel(pixel)) {
        return;
    }

    const auto pixel_index = static_cast<std::size_t>(pixel.y) * _pixel_width + static_cast<std::size_t>(pixel.x);
    _pixels[pixel_index] = c.blend_over(Color::from_rgba(_pixels[pixel_index])).rgba();
}

bool Canvas::show() {
    if (_pixel_width == 0 || _pixel_height == 0) {
        return true;
    }

    if (_texture == nullptr) {
        const auto max_dimension = static_cast<unsigned int>(std::numeric_limits<int>::max());
        if (_pixel_width > max_dimension || _pixel_height > max_dimension) {
            return SDL_SetError("Canvas dimensions exceed SDL texture dimension limits.");
        }

        _texture = SDL_CreateTexture(_renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                    static_cast<int>(_pixel_width), static_cast<int>(_pixel_height));
        if (_texture == nullptr) {
            return false;
        }
        if (!SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND)
            || !SDL_SetTextureScaleMode(_texture, SDL_SCALEMODE_NEAREST)) {
            SDL_DestroyTexture(_texture);
            _texture = nullptr;
            return false;
        }
    }

    void* texture_pixels = nullptr;
    int pitch = 0;
    if (!SDL_LockTexture(_texture, nullptr, &texture_pixels, &pitch)) {
        return false;
    }

    const auto row_size = static_cast<std::size_t>(_pixel_width) * sizeof(std::uint32_t);
    for (std::size_t y = 0; y < _pixel_height; ++y) {
        std::memcpy(static_cast<unsigned char*>(texture_pixels) + y * pitch,
                    _pixels + y * _pixel_width, row_size);
    }
    SDL_UnlockTexture(_texture);

    return SDL_SetRenderTarget(_renderer.get(), nullptr)
        && SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255)
        && SDL_RenderClear(_renderer.get())
        && SDL_RenderTexture(_renderer.get(), _texture, nullptr, nullptr)
        && SDL_RenderPresent(_renderer.get());
}

