module;

#include <cmath>
#include <cstring>
#include <limits>
#include <new>
#include <utility>
#include <SDL3/SDL.h>

module canvas;

Canvas::Canvas(std::shared_ptr<SDL_Renderer> renderer, const std::uint32_t pixel_width, const std::uint32_t pixel_height)
    : _renderer(std::move(renderer)),
      _pixel_width(pixel_width),
      _pixel_height(pixel_height) {
    const auto width = static_cast<std::size_t>(pixel_width);
    const auto height = static_cast<std::size_t>(pixel_height);
    if(height != 0 && width > std::numeric_limits<std::size_t>::max() / sizeof(std::uint32_t) / height) {
        throw std::bad_array_new_length{};
    }

    // Each pixel represents 0xRRGGBBAA; its byte order depends on the platform.
    _pixels = new std::uint32_t[width * height]{};
}

Canvas::~Canvas() {
    SDL_DestroyTexture(_texture);
    delete[] _pixels;
}

bool Canvas::use_centered_coordinates(const bool enabled) {
    _centered_coordinates = enabled;
    return _centered_coordinates;
}

void Canvas::set_pixel(const int x,    const int y,    const Color c) { _set_raw_pixel(_convert_coords(x, y), c); }
void Canvas::set_pixel(const double x, const double y, const Color c) { _set_raw_pixel(_convert_coords(x, y), c); }

Color Canvas::get_pixel(const int x,    const int y)    const { return _get_raw_pixel(_convert_coords(x, y)); }
Color Canvas::get_pixel(const double x, const double y) const { return _get_raw_pixel(_convert_coords(x, y)); }

std::pair<std::int64_t, std::int64_t> Canvas::_convert_coords(int x, int y) const {
    std::int64_t screen_x = x;
    std::int64_t screen_y = y;
    if(_centered_coordinates) {
        screen_x = static_cast<std::int64_t>(_pixel_width / 2) + x;
        screen_y = (static_cast<std::int64_t>(_pixel_height) - 1) / 2 - y;
    }
    return {screen_x, screen_y};
}

std::pair<std::int64_t, std::int64_t> Canvas::_convert_coords(double x, double y) const {
    // Sanity check. All coords much be in range of [0.0, 1.0], or [-1.0, 1.0].
    // Reject invalid inputs before rounding; NaN and infinity cannot become pixel indices.
    // All valid raw pixel coords are in the range of [0, size - 1]
    const double coordinate_min = _centered_coordinates ? -1.0 : 0.0;
    if(!(x >= coordinate_min && x <= 1.0 && y >= coordinate_min && y <= 1.0)) {
        return {-1, -1}; // Never valid for raw pixel coordinates.
    }
    if(_centered_coordinates) {
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

bool Canvas::_contains_pixel(std::pair<std::int64_t, std::int64_t> pixel) const {
    const auto [x, y] = pixel;
    return x >= 0 && y >= 0 && x < _pixel_width && y < _pixel_height;
}

void Canvas::_set_raw_pixel(std::pair<std::int64_t, std::int64_t> position, const Color c) {
    if(!_contains_pixel(position)) { return; }

    const auto [x, y] = position;
    const auto pixel_index = static_cast<std::size_t>(y) * _pixel_width + static_cast<std::size_t>(x);
    _pixels[pixel_index] = c.blend_over(Color::from_rgba(_pixels[pixel_index])).rgba();
}

Color Canvas::_get_raw_pixel(std::pair<std::int64_t, std::int64_t> position) const {
    if(!_contains_pixel(position)) { return {}; }
    const auto [x, y] = position;
    return Color::from_rgba(_pixels[static_cast<std::size_t>(y) * _pixel_width + static_cast<std::size_t>(x)]);
}

bool Canvas::show() {
    if(_pixel_width == 0 || _pixel_height == 0) { return true; }

    if(_texture == nullptr) {
        constexpr auto max_dimension = static_cast<unsigned int>(std::numeric_limits<int>::max());
        if(_pixel_width > max_dimension || _pixel_height > max_dimension) {
            return SDL_SetError("Canvas dimensions exceed SDL texture dimension limits.");
        }

        _texture = SDL_CreateTexture(_renderer.get(),
                                     SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     static_cast<int>(_pixel_width),
                                     static_cast<int>(_pixel_height)
        );
        if(_texture == nullptr) { return false; }

        if(!SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND)
            || !SDL_SetTextureScaleMode(_texture, SDL_SCALEMODE_NEAREST)) {
            SDL_DestroyTexture(_texture);
            _texture = nullptr;
            return false;
        }
    }

    void* texture_pixels = nullptr;
    int pitch = 0;
    if(!SDL_LockTexture(_texture, nullptr, &texture_pixels, &pitch)) { return false; }

    const auto row_size = static_cast<std::size_t>(_pixel_width) * sizeof(std::uint32_t);
    for(std::size_t y = 0; y < _pixel_height; ++y) {
        std::memcpy(static_cast<unsigned char*>(texture_pixels) + y * pitch,
                    _pixels + y * _pixel_width,
                    row_size);
    }
    SDL_UnlockTexture(_texture);

    return SDL_SetRenderTarget(_renderer.get(), nullptr)
        && SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255)
        && SDL_RenderClear(_renderer.get())
        && SDL_RenderTexture(_renderer.get(), _texture, nullptr, nullptr)
        && SDL_RenderPresent(_renderer.get());
}
