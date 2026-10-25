module;

#include <cstdint>
#include <memory>
#include <SDL3/SDL_render.h>

export module canvas;

export import Color;

/** @brief Canvas interface for setting individual pixels. */
export class Canvas {
public:
    /**
     * @brief Allocates a zero-initialized buffer of packed RGBA8888 pixels.
     * @param renderer Shared ownership of the window renderer and its lifetime dependencies.
     * @pre renderer is not null.
     * @param pixel_width Width in pixels; may be zero.
     * @param pixel_height Height in pixels; may be zero.
     * @throws std::bad_array_new_length If the buffer size would overflow.
     * @throws std::bad_alloc If memory allocation fails.
     */
    Canvas(std::shared_ptr<SDL_Renderer> renderer, unsigned int pixel_width, unsigned int pixel_height);

    /** @brief Releases the pixel buffer and texture before the renderer is destroyed. */
    ~Canvas();

    /** @brief Copying is disabled because the canvas owns its pixel buffer. */
    Canvas(const Canvas&) = delete;
    /** @brief Copy assignment is disabled because the canvas owns its pixel buffer. */
    Canvas& operator=(const Canvas&) = delete;

    /**
     * @brief Composites a color over a pixel; ignores out-of-bounds coordinates.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param c Foreground color; the existing pixel is the background.
     */
    void set_pixel(int x, int y, Color c);

    /**
     * @brief Composites a pixel at normalized image coordinates, rounded to the nearest pixel.
     * @param x Horizontal coordinate in [0, 1], mapping to [0, width - 1].
     * @param y Vertical coordinate in [0, 1], mapping to [0, height - 1].
     * @param c Foreground color; the existing pixel is the background.
     * @note Nonfinite or out-of-range coordinates and empty canvases are ignored.
     */
    void set_pixel(double x, double y, Color c);

    /**
     * @brief Uploads the pixels, clears the window to black, draws, and presents the frame.
     * @return True on success, or false on SDL failure; see SDL_GetError().
     * @note Call on the main thread. Zero-sized canvases do nothing and return true.
     */
    bool show();

private:
    /** @brief Composites a pixel at raw image coordinates; ignores out-of-bounds coordinates. */
    void _set_raw_pixel(unsigned int x, unsigned int y, Color c);

    std::shared_ptr<SDL_Renderer> _renderer;
    SDL_Texture* _texture = nullptr;
    unsigned int _pixel_width;
    unsigned int _pixel_height;
    std::uint32_t* _pixels;
};
