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
     * @brief Placeholder for setting a pixel; currently has no effect.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param c Pixel color.
     */
    void set_pixel(int x, int y, Color c);
};
