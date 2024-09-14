/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2024 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef GOLD_CARTRIDGE_FONT_MANAGER_H
#define GOLD_CARTRIDGE_FONT_MANAGER_H

#include <memory>

#include <SDL_ttf.h>

namespace Core {

    class FontManager {
    public:
        using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
        using FontPtr = std::shared_ptr<TTF_Font>;

        int default_font_size();
        SDL_Color default_font_color();
    public:
        static FontManager& access();
        static bool is_initialized();

        bool start_up();
        void shut_down();

        FontPtr default_font();
        TexturePtr render_text(SDL_Renderer* renderer, const std::string& text,
                               const FontPtr& font, const SDL_Color& font_color);

        FontManager(const FontManager&) = delete;
        void operator=(const FontManager&) = delete;

    private:
        FontManager();
        ~FontManager();
    };

} // Core

#endif //GOLD_CARTRIDGE_FONT_MANAGER_H
