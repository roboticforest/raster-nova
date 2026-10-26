module;

#include <memory>
#include <utility>
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
    Canvas(std::shared_ptr<SDL_Renderer> renderer, std::uint32_t pixel_width, std::uint32_t pixel_height);

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
     * @note Uses top-left coordinates by default, or centered coordinates when enabled.
     */
    void set_pixel(int x, int y, Color c);

    /**
     * @brief Composites a pixel at normalized image coordinates, rounded to the nearest pixel.
     * @param x Horizontal coordinate in [0, 1], or [-1, 1] in centered mode.
     * @param y Vertical coordinate in [0, 1], or [-1, 1] in centered mode.
     * @param c Foreground color; the existing pixel is the background.
     * @note Nonfinite or out-of-range coordinates and empty canvases are ignored.
     *       Endpoints map to the outermost pixels. Centered mode uses Y-up coordinates,
     *       with rounding before the Y flip so (0.0, 0.0) matches the integer origin.
     */
    void set_pixel(double x, double y, Color c);

    /**
     * @brief Reads a pixel using the same coordinate mode as the integer setter.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @return The stored color, or transparent black if the coordinates are out of bounds.
     */
    Color get_pixel(int x, int y) const;

    /**
     * @brief Reads a pixel using the same normalized coordinates and rounding as the floating-point setter.
     * @param x Horizontal coordinate in [0, 1], or [-1, 1] in centered mode.
     * @param y Vertical coordinate in [0, 1], or [-1, 1] in centered mode.
     * @return The stored color, or transparent black for invalid coordinates or an empty canvas.
     */
    Color get_pixel(double x, double y) const;

    /**
     * @brief Selects centered, Y-up coordinates for pixel reads and writes without changing pixels.
     * @param enabled True for centered coordinates, false for top-left, Y-down coordinates.
     * @return The current centered-coordinate setting.
     * @note Each integer axis ranges from -floor(size / 2) through ceil(size / 2) - 1.
     *       Maps x to width / 2 + x and y to (height - 1) / 2 - y using integer division.
     */
    bool use_centered_coordinates(bool enabled = true);

    /**
     * @brief Uploads the pixels, clears the window to black, draws, and presents the frame.
     * @return True on success, or false on SDL failure; see SDL_GetError().
     * @note Call on the main thread. Zero-sized canvases do nothing and return true.
     */
    bool show();

private:

    /**
     * @brief Converts integer coordinates in the current mode to raw top-left, Y-down pixels.
     * @param x Horizontal pixel coordinate in the current mode.
     * @param y Vertical pixel coordinate in the current mode.
     * @return Raw coordinates; no bounds checking or clipping is performed.
     */
    std::pair<std::int64_t, std::int64_t> _convert_coords(int x, int y) const;

    /**
     * @brief Converts normalized coordinates to raw pixels, rounding before any centered Y flip.
     * @param x Normalized horizontal coordinate in the current mode.
     * @param y Normalized vertical coordinate in the current mode.
     * @return Raw coordinates, or (-1, -1) for invalid normalized input.
     * @note Endpoints map to the outermost pixels, matching the floating-point pixel functions.
     *       Axes with zero or one pixel map to 0; an empty canvas still has no valid pixels.
     */
    std::pair<std::int64_t, std::int64_t> _convert_coords(double x, double y) const;

    /** @brief Tests whether raw coordinates identify a pixel in the buffer. */
    bool _contains_pixel(std::pair<std::int64_t, std::int64_t> pixel) const;

    /** @brief Composites a pixel at raw image coordinates; ignores out-of-bounds coordinates. */
    void _set_raw_pixel(std::pair<std::int64_t, std::int64_t> position, Color c);

    /** @brief Reads a raw pixel, returning transparent black if it is out of bounds. */
    Color _get_raw_pixel(std::pair<std::int64_t, std::int64_t> position) const;

    // Data
    std::shared_ptr<SDL_Renderer> _renderer;
    SDL_Texture* _texture = nullptr;
    std::uint32_t _pixel_width;
    std::uint32_t _pixel_height;
    std::uint32_t* _pixels;
    bool _centered_coordinates = false;
};
