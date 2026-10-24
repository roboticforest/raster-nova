module;

#include <cstdint>

export module canvas;

/** @brief Temporary RGBA color until the Color module is introduced. */
export struct Color {
    int red;
    int green;
    int blue;
    int alpha;
};

/** @brief Canvas interface for setting individual pixels. */
export class Canvas {
public:
    /**
     * @brief Allocates a zero-initialized buffer of packed RGBA8888 pixels.
     * @param pixel_width Width in pixels.
     * @param pixel_height Height in pixels.
     * @throws std::invalid_argument If either dimension is not positive.
     * @throws std::length_error If the buffer size would overflow.
     */
    Canvas(int pixel_width, int pixel_height);

    /** @brief Releases the pixel buffer. */
    ~Canvas();

    /** @brief Copying is disabled because the canvas owns its pixel buffer. */
    Canvas(const Canvas&) = delete;
    /** @brief Copy assignment is disabled because the canvas owns its pixel buffer. */
    Canvas& operator=(const Canvas&) = delete;

    /**
     * @brief Placeholder for setting a pixel; currently has no effect.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param c Pixel color.
     */
    void set_pixel(int x, int y, Color c);

private:
    int _pixel_width;
    int _pixel_height;
    std::uint32_t* _pixels;
};
