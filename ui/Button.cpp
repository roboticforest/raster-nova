/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2023-2024 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Button.h"
#include "../core/FontManager.h"

#include <SDL_ttf.h>
#include <SDL_render.h>

#include <utility>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
/// Default Values
////////////////////////////////////////////////////////////////////////////////

namespace {
    const SDL_Color DEFAULT_BUTTON_BASE_COLOR{224, 224, 224, 255};
    const SDL_Color DEFAULT_BUTTON_HIGHLIGHT_COLOR{255, 255, 255, 255};
}

////////////////////////////////////////////////////////////////////////////////
/// General Helper Functions
////////////////////////////////////////////////////////////////////////////////

static int SET_RENDER_DRAW_COLOR(SDL_Renderer* renderer, const SDL_Color& draw_color) {
    return SDL_SetRenderDrawColor(renderer, draw_color.r, draw_color.g, draw_color.b, draw_color.a);
}

////////////////////////////////////////////////////////////////////////////////
/// Constructors and Destructors
////////////////////////////////////////////////////////////////////////////////

UI::Button::Button(int x_pixel_pos,
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
) : m_button_area{x_pixel_pos, y_pixel_pos, pixel_width, pixel_height},
    m_button_label(std::move(text_label)),
    m_button_action(std::move(click_action)),
    m_label_font(std::move(label_font)),
    m_label_font_size_pt(label_font_size_pt),
    m_label_font_color(label_font_color),
    m_button_highlight_color(highlight_color),
    m_button_color(button_color),
    m_button_is_depressed(false),
    m_button_is_highlighted(false) {}

UI::Button::Button(int x_pixel_pos,
                   int y_pixel_pos,
                   int pixel_width,
                   int pixel_height,
                   std::string text_label,
                   UI::Button::Action click_action
) : Button(x_pixel_pos,
           y_pixel_pos,
           pixel_width,
           pixel_height,
           text_label,
           click_action,
           Core::FontManager::access().default_font(),
           Core::FontManager::access().default_font_size(),
           Core::FontManager::access().default_font_color(),
           DEFAULT_BUTTON_HIGHLIGHT_COLOR,
           DEFAULT_BUTTON_BASE_COLOR) {}

////////////////////////////////////////////////////////////////////////////////
/// Public API Functions
////////////////////////////////////////////////////////////////////////////////

void UI::Button::handle_event(SDL_Event& event) {
    switch (event.type) {
        case SDL_MOUSEMOTION: {
            m_button_is_highlighted = button_contains_point(event.motion.x, event.motion.y);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            if (button_contains_point(event.button.x, event.button.y)) {
                m_button_is_depressed = true;
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            if (m_button_is_depressed && button_contains_point(event.button.x, event.button.y)) {
                m_button_action(*this);
            }
            m_button_is_depressed = false;
            break;
        }
    }
}

void UI::Button::render(SDL_Renderer* renderer) {
    // Fill the button area with color.
    const SDL_Color& fill_color = m_button_is_highlighted ? m_button_highlight_color : m_button_color;
    SET_RENDER_DRAW_COLOR(renderer, fill_color);
    SDL_RenderFillRect(renderer, &m_button_area);

    // Render the text as an image.
    using Fonts = Core::FontManager;
    Fonts::TexturePtr text_render = Fonts::access().render_text(renderer, m_button_label, m_label_font, m_label_font_color);

    // Calculate the label's drawing area within the button.
    int texture_w{}, texture_h{};
    SDL_QueryTexture(text_render.get(), nullptr, nullptr, &texture_w, &texture_h);
    SDL_Rect label_dest_px = {
            std::max(m_button_area.x, m_button_area.x + (m_button_area.w / 2) - (texture_w / 2)),
            std::max(m_button_area.y, m_button_area.y + (m_button_area.h / 2) - (texture_h / 2)),
            std::min(texture_w, m_button_area.w),
            std::min(texture_h, m_button_area.h)
    };

    // Calculate which pixels of the texture to copy into the label drawing area on screen.
    int      h_clip       = std::max(texture_w - m_button_area.w, 0);
    int      v_clip       = std::max(texture_h - m_button_area.h, 0);
    SDL_Rect label_src_px = {h_clip / 2, v_clip / 2, texture_w - h_clip, texture_h - v_clip};

    // Render the button label to the screen.
    SDL_RenderCopy(renderer, text_render.get(), &label_src_px, &label_dest_px);
    SDL_DestroyTexture(text_render.get());
}


void UI::Button::set_label(std::string new_label) {
    m_button_label = std::move(new_label);
}

////////////////////////////////////////////////////////////////////////////////
/// Private API Functions
////////////////////////////////////////////////////////////////////////////////

bool UI::Button::button_contains_point(int x, int y) const {
    return x >= m_button_area.x &&
           x <= m_button_area.x + m_button_area.w &&
           y >= m_button_area.y &&
           y <= m_button_area.y + m_button_area.h;
}


