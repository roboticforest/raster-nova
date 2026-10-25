module;

#include <cstddef>
#include <cstdint>
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
    if (x < 0 || y < 0 || static_cast<unsigned int>(x) >= _pixel_width
        || static_cast<unsigned int>(y) >= _pixel_height) {
        return;
    }

    const auto pixel_index = static_cast<std::size_t>(y) * _pixel_width + static_cast<std::size_t>(x);
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

