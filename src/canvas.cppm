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
     * @brief Creates a canvas with the given pixel dimensions.
     * @param pixel_width Width in pixels.
     * @param pixel_height Height in pixels.
     */
    Canvas(int pixel_width, int pixel_height);

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
};
