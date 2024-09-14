/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2023 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef GOLD_CARTRIDGE_BUTTON_H
#define GOLD_CARTRIDGE_BUTTON_H

#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <functional>
#include <memory>
#include <string>


namespace UI {

/**
 * @brief Represents a button that can be displayed on the screen.
 */
    class Button {
    public:
        typedef std::function<void(Button& this_button)> Action;

    public:
        /**
         * Constructs and configures a Button object.
         * @param x_pixel_pos The x-coordinate of the button's top-left corner, in pixels.
         * @param y_pixel_pos The y-coordinate of the button's top-left corner, in pixels.
         * @param pixel_width The width of the button, in pixels.
         * @param pixel_height The height of the button, in pixels.
         * @param text_label The label displayed on the button.
         * @param click_action The function to be performed when the button is clicked.
         * @param label_font A pointer to the font used to render the button's label text.
         * @param label_font_size_pt The height of the button's label font, in points.
         * @param label_font_color The color of the button's label text.
         * @param highlight_color The color of the button when the mouse hovers over it.
         * @param button_color The base color of the button when the mouse is outside of it.
         */
        Button(int x_pixel_pos,
               int y_pixel_pos,
               int pixel_width,
               int pixel_height,
               std::string text_label,
               Action click_action,
               std::shared_ptr<TTF_Font> label_font,
               int label_font_size_pt,
               SDL_Color label_font_color,
               SDL_Color highlight_color,
               SDL_Color button_color
        );
        Button(int x_pixel_pos,
               int y_pixel_pos,
               int pixel_width,
               int pixel_height,
               std::string text_label,
               Action click_action
        );

        void handle_event(SDL_Event& event);

        void render(SDL_Renderer* renderer);

        void set_label(std::string new_label);

    private: // Functions
        /**
         * Checks whether a pixel coordinate is within the button's area.
         * @param x The x-coordinate of the point, in pixels.
         * @param y The y-coordinate of the point, in pixels.
         * @return True if the point is within the button's area, false otherwise.
         */
        bool button_contains_point(int x, int y) const;

    private: // Data fields
        SDL_Rect  m_button_area;
        SDL_Color m_button_color;
        SDL_Color m_button_highlight_color;

        bool m_button_is_depressed;
        bool m_button_is_highlighted;

        Action                    m_button_action;
        std::string               m_button_label;
        std::shared_ptr<TTF_Font> m_label_font;
        SDL_Color                 m_label_font_color;
        int                       m_label_font_size_pt;
    };

} // UI namespace

#endif //GOLD_CARTRIDGE_BUTTON_H
