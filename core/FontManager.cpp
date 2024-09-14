/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2024 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "FontManager.h"

#include <SDL_render.h>

#include <iostream>

namespace Core {

////////////////////////////////////////////////////////////////////////////////
/// Translation Unit Variables
////////////////////////////////////////////////////////////////////////////////

    namespace {
        std::shared_ptr<TTF_Font> DEFAULT_FONT;
        const std::string         DEFAULT_FONT_FILE    = "resources/fonts/playfair-display-font/PlayfairDisplayRegular-ywLOY.ttf";
        const int                 DEFAULT_FONT_PT_SIZE = 12;
        const SDL_Color           DEFAULT_FONT_COLOR{0, 0, 0, 255};
        bool                      MANAGER_INITIALIZED  = false;
    }

////////////////////////////////////////////////////////////////////////////////
/// Public API FontManager Functions
////////////////////////////////////////////////////////////////////////////////

    bool FontManager::is_initialized() {
        return MANAGER_INITIALIZED;
    }

    bool FontManager::start_up() {
        if (!is_initialized()) {
            TTF_Font* font = TTF_OpenFont(DEFAULT_FONT_FILE.c_str(), DEFAULT_FONT_PT_SIZE);
            DEFAULT_FONT = std::shared_ptr<TTF_Font>(font, TTF_CloseFont);

            MANAGER_INITIALIZED = true;
        }
        return MANAGER_INITIALIZED;
    }

    void FontManager::shut_down() {
        if (is_initialized()) {
            DEFAULT_FONT.reset();
            MANAGER_INITIALIZED = false;
        }
    }

    [[maybe_unused]] FontManager& FontManager::access() {
        static FontManager instance;
        return instance;
    }

    std::shared_ptr<TTF_Font> FontManager::default_font() { return DEFAULT_FONT; }

    int FontManager::default_font_size() { return DEFAULT_FONT_PT_SIZE; }

    SDL_Color FontManager::default_font_color() { return DEFAULT_FONT_COLOR; }

    FontManager::TexturePtr
    FontManager::render_text(SDL_Renderer* renderer, const std::string& text,
                             const FontPtr& font, const SDL_Color& font_color) {
        // Pre-render the text to a pixel surface.
        SDL_Surface* prerender = TTF_RenderText_Blended(font.get(), text.c_str(), font_color);
        if (!prerender) {
            std::cerr << "Unable to render text to a drawing surface." << std::endl;
            return {nullptr, SDL_DestroyTexture};
        }

        // Render the surface to a texture.
        SDL_Texture* final_render = SDL_CreateTextureFromSurface(renderer, prerender);
        SDL_FreeSurface(prerender);
        if (!final_render) {
            std::cerr << "Unable to convert text drawing surface to a texture." << std::endl;
            return {nullptr, SDL_DestroyTexture};
        }

        return TexturePtr{final_render, SDL_DestroyTexture};
    }

////////////////////////////////////////////////////////////////////////////////
/// Private API
////////////////////////////////////////////////////////////////////////////////

    FontManager::FontManager() = default;
    FontManager::~FontManager() { shut_down(); }

} // Core