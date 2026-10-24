module canvas;

Canvas::Canvas(int pixel_width, int pixel_height)
    : _pixel_width(pixel_width), _pixel_height(pixel_height) {
}

void Canvas::set_pixel(int x, int y, Color c) {
    // Pixel storage and rendering will be implemented later.
    (void)x;
    (void)y;
    (void)c;
}

