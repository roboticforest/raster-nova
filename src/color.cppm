/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2023 David Vitez
 * @par License:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

module;

#include <cstdint>

export module Color;

/** @brief Color stored as a packed 0xRRGGBBAA value. */
export class Color {
public:
    /** @brief Creates transparent black. */
    constexpr Color() noexcept = default;

    /** @brief Creates a color using from_rgba(). @param value Packed 0xRRGGBBAA color. */
    constexpr explicit Color(std::uint32_t value) noexcept : Color(from_rgba(value)) {}

    /**
     * @brief Packs byte channels into an RGBA color.
     * @param red Red channel.
     * @param green Green channel.
     * @param blue Blue channel.
     * @param alpha Alpha channel; defaults to fully opaque and is stored without blending.
     */
    constexpr Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255) noexcept
        : _rgba((static_cast<std::uint32_t>(red) << 24)
              | (static_cast<std::uint32_t>(green) << 16)
              | (static_cast<std::uint32_t>(blue) << 8)
              | static_cast<std::uint32_t>(alpha)) {
    }

    /**
     * @brief Creates an opaque color from a packed RGB value.
     * @param value Packed 0xRRGGBB color; only the low 24 bits are used.
     * @return The RGB color with alpha set to 255.
     */
    static constexpr Color from_rgb(std::uint32_t value) noexcept {
        return from_rgba((value << 8) | 0xFFu);
    }

    /**
     * @brief Creates a color from a packed RGBA value.
     * @param value Packed 0xRRGGBBAA color.
     * @return The color with all four channels preserved.
     */
    static constexpr Color from_rgba(std::uint32_t value) noexcept {
        Color color;
        color.rgba(value);
        return color;
    }

    /**
     * @name Channel getters
     * @brief Return the selected channel in the range 0-255.
     * @{
     */
    constexpr std::uint8_t red() const noexcept { return static_cast<std::uint8_t>(_rgba >> 24); }
    constexpr std::uint8_t green() const noexcept { return static_cast<std::uint8_t>(_rgba >> 16); }
    constexpr std::uint8_t blue() const noexcept { return static_cast<std::uint8_t>(_rgba >> 8); }
    constexpr std::uint8_t alpha() const noexcept { return static_cast<std::uint8_t>(_rgba); }
    /** @} */

    /**
     * @name Channel setters
     * @brief Replace one channel, preserving the other channels.
     * @param value New byte channel value.
     * @{
     */
    constexpr void red(std::uint8_t value) noexcept { _rgba = (_rgba & 0x00FFFFFFu) | (static_cast<std::uint32_t>(value) << 24); }
    constexpr void green(std::uint8_t value) noexcept { _rgba = (_rgba & 0xFF00FFFFu) | (static_cast<std::uint32_t>(value) << 16); }
    constexpr void blue(std::uint8_t value) noexcept { _rgba = (_rgba & 0xFFFF00FFu) | (static_cast<std::uint32_t>(value) << 8); }
    constexpr void alpha(std::uint8_t value) noexcept { _rgba = (_rgba & 0xFFFFFF00u) | static_cast<std::uint32_t>(value); }
    /** @} */

    /** @brief Returns the packed 0xRRGGBBAA value. */
    constexpr std::uint32_t rgba() const noexcept { return _rgba; }

    /** @brief Replaces all channels with the supplied packed 0xRRGGBBAA value. @param value Packed color. */
    constexpr void rgba(std::uint32_t value) noexcept { _rgba = value; }

private:
    std::uint32_t _rgba = 0;
};

/**
 * @name Named web colors
 * @brief Convenience functions returning the 148 named web colors.
 * @return The named Color with fully opaque alpha (255).
 * @see https://www.w3schools.com/colors/colors_names.asp for the color names and RGB values.
 * @{
 */
export namespace colors {
    constexpr Color alice_blue()                   noexcept { return Color{240, 248, 255, 255}; }
    constexpr Color antique_white()                noexcept { return Color{250, 235, 215, 255}; }
    constexpr Color aqua()                         noexcept { return Color{0, 255, 255, 255}; }
    constexpr Color aquamarine()                   noexcept { return Color{127, 255, 212, 255}; }
    constexpr Color azure()                        noexcept { return Color{240, 255, 255, 255}; }
    constexpr Color beige()                        noexcept { return Color{245, 245, 220, 255}; }
    constexpr Color bisque()                       noexcept { return Color{255, 228, 196, 255}; }
    constexpr Color black()                        noexcept { return Color{0, 0, 0, 255}; }
    constexpr Color blanched_almond()              noexcept { return Color{255, 235, 205, 255}; }
    constexpr Color blue()                         noexcept { return Color{0, 0, 255, 255}; }
    constexpr Color blue_violet()                  noexcept { return Color{138, 43, 226, 255}; }
    constexpr Color brown()                        noexcept { return Color{165, 42, 42, 255}; }
    constexpr Color burly_wood()                   noexcept { return Color{222, 184, 135, 255}; }
    constexpr Color cadet_blue()                   noexcept { return Color{95, 158, 160, 255}; }
    constexpr Color chartreuse()                   noexcept { return Color{127, 255, 0, 255}; }
    constexpr Color chocolate()                    noexcept { return Color{210, 105, 30, 255}; }
    constexpr Color coral()                        noexcept { return Color{255, 127, 80, 255}; }
    constexpr Color cornflower_blue()              noexcept { return Color{100, 149, 237, 255}; }
    constexpr Color cornsilk()                     noexcept { return Color{255, 248, 220, 255}; }
    constexpr Color crimson()                      noexcept { return Color{220, 20, 60, 255}; }
    constexpr Color cyan()                         noexcept { return Color{0, 255, 255, 255}; }
    constexpr Color dark_blue()                    noexcept { return Color{0, 0, 139, 255}; }
    constexpr Color dark_cyan()                    noexcept { return Color{0, 139, 139, 255}; }
    constexpr Color dark_golden_rod()              noexcept { return Color{184, 134, 11, 255}; }
    constexpr Color dark_gray()                    noexcept { return Color{169, 169, 169, 255}; }
    constexpr Color dark_green()                   noexcept { return Color{0, 100, 0, 255}; }
    constexpr Color dark_grey()                    noexcept { return Color{169, 169, 169, 255}; }
    constexpr Color dark_khaki()                   noexcept { return Color{189, 183, 107, 255}; }
    constexpr Color dark_magenta()                 noexcept { return Color{139, 0, 139, 255}; }
    constexpr Color dark_olive_green()             noexcept { return Color{85, 107, 47, 255}; }
    constexpr Color dark_orange()                  noexcept { return Color{255, 140, 0, 255}; }
    constexpr Color dark_orchid()                  noexcept { return Color{153, 50, 204, 255}; }
    constexpr Color dark_red()                     noexcept { return Color{139, 0, 0, 255}; }
    constexpr Color dark_salmon()                  noexcept { return Color{233, 150, 122, 255}; }
    constexpr Color dark_sea_green()               noexcept { return Color{143, 188, 143, 255}; }
    constexpr Color dark_slate_blue()              noexcept { return Color{72, 61, 139, 255}; }
    constexpr Color dark_slate_gray()              noexcept { return Color{47, 79, 79, 255}; }
    constexpr Color dark_slate_grey()              noexcept { return Color{47, 79, 79, 255}; }
    constexpr Color dark_turquoise()               noexcept { return Color{0, 206, 209, 255}; }
    constexpr Color dark_violet()                  noexcept { return Color{148, 0, 211, 255}; }
    constexpr Color deep_pink()                    noexcept { return Color{255, 20, 147, 255}; }
    constexpr Color deep_sky_blue()                noexcept { return Color{0, 191, 255, 255}; }
    constexpr Color dim_gray()                     noexcept { return Color{105, 105, 105, 255}; }
    constexpr Color dim_grey()                     noexcept { return Color{105, 105, 105, 255}; }
    constexpr Color dodger_blue()                  noexcept { return Color{30, 144, 255, 255}; }
    constexpr Color firebrick()                    noexcept { return Color{178, 34, 34, 255}; }
    constexpr Color floral_white()                 noexcept { return Color{255, 250, 240, 255}; }
    constexpr Color forest_green()                 noexcept { return Color{34, 139, 34, 255}; }
    constexpr Color fuchsia()                      noexcept { return Color{255, 0, 255, 255}; }
    constexpr Color gainsboro()                    noexcept { return Color{220, 220, 220, 255}; }
    constexpr Color ghost_white()                  noexcept { return Color{248, 248, 255, 255}; }
    constexpr Color gold()                         noexcept { return Color{255, 215, 0, 255}; }
    constexpr Color golden_rod()                   noexcept { return Color{218, 165, 32, 255}; }
    constexpr Color gray()                         noexcept { return Color{128, 128, 128, 255}; }
    constexpr Color green()                        noexcept { return Color{0, 128, 0, 255}; }
    constexpr Color green_yellow()                 noexcept { return Color{173, 255, 47, 255}; }
    constexpr Color grey()                         noexcept { return Color{128, 128, 128, 255}; }
    constexpr Color honeydew()                     noexcept { return Color{240, 255, 240, 255}; }
    constexpr Color hot_pink()                     noexcept { return Color{255, 105, 180, 255}; }
    constexpr Color indian_red()                   noexcept { return Color{205, 92, 92, 255}; }
    constexpr Color indigo()                       noexcept { return Color{75, 0, 130, 255}; }
    constexpr Color ivory()                        noexcept { return Color{255, 255, 240, 255}; }
    constexpr Color khaki()                        noexcept { return Color{240, 230, 140, 255}; }
    constexpr Color lavender()                     noexcept { return Color{230, 230, 250, 255}; }
    constexpr Color lavender_blush()               noexcept { return Color{255, 240, 245, 255}; }
    constexpr Color lawn_green()                   noexcept { return Color{124, 252, 0, 255}; }
    constexpr Color lemon_chiffon()                noexcept { return Color{255, 250, 205, 255}; }
    constexpr Color light_blue()                   noexcept { return Color{173, 216, 230, 255}; }
    constexpr Color light_coral()                  noexcept { return Color{240, 128, 128, 255}; }
    constexpr Color light_cyan()                   noexcept { return Color{224, 255, 255, 255}; }
    constexpr Color light_golden_rod_yellow()      noexcept { return Color{250, 250, 210, 255}; }
    constexpr Color light_gray()                   noexcept { return Color{211, 211, 211, 255}; }
    constexpr Color light_green()                  noexcept { return Color{144, 238, 144, 255}; }
    constexpr Color light_grey()                   noexcept { return Color{211, 211, 211, 255}; }
    constexpr Color light_pink()                   noexcept { return Color{255, 182, 193, 255}; }
    constexpr Color light_salmon()                 noexcept { return Color{255, 160, 122, 255}; }
    constexpr Color light_sea_green()              noexcept { return Color{32, 178, 170, 255}; }
    constexpr Color light_sky_blue()               noexcept { return Color{135, 206, 250, 255}; }
    constexpr Color light_slate_gray()             noexcept { return Color{119, 136, 153, 255}; }
    constexpr Color light_slate_grey()             noexcept { return Color{119, 136, 153, 255}; }
    constexpr Color light_steel_blue()             noexcept { return Color{176, 196, 222, 255}; }
    constexpr Color light_yellow()                 noexcept { return Color{255, 255, 224, 255}; }
    constexpr Color lime()                         noexcept { return Color{0, 255, 0, 255}; }
    constexpr Color lime_green()                   noexcept { return Color{50, 205, 50, 255}; }
    constexpr Color linen()                        noexcept { return Color{250, 240, 230, 255}; }
    constexpr Color magenta()                      noexcept { return Color{255, 0, 255, 255}; }
    constexpr Color maroon()                       noexcept { return Color{128, 0, 0, 255}; }
    constexpr Color medium_aquamarine()            noexcept { return Color{102, 205, 170, 255}; }
    constexpr Color medium_blue()                  noexcept { return Color{0, 0, 205, 255}; }
    constexpr Color medium_orchid()                noexcept { return Color{186, 85, 211, 255}; }
    constexpr Color medium_purple()                noexcept { return Color{147, 112, 219, 255}; }
    constexpr Color medium_sea_green()             noexcept { return Color{60, 179, 113, 255}; }
    constexpr Color medium_slate_blue()            noexcept { return Color{123, 104, 238, 255}; }
    constexpr Color medium_spring_green()          noexcept { return Color{0, 250, 154, 255}; }
    constexpr Color medium_turquoise()             noexcept { return Color{72, 209, 204, 255}; }
    constexpr Color medium_violet_red()            noexcept { return Color{199, 21, 133, 255}; }
    constexpr Color midnight_blue()                noexcept { return Color{25, 25, 112, 255}; }
    constexpr Color mint_cream()                   noexcept { return Color{245, 255, 250, 255}; }
    constexpr Color misty_rose()                   noexcept { return Color{255, 228, 225, 255}; }
    constexpr Color moccasin()                     noexcept { return Color{255, 228, 181, 255}; }
    constexpr Color navajo_white()                 noexcept { return Color{255, 222, 173, 255}; }
    constexpr Color navy()                         noexcept { return Color{0, 0, 128, 255}; }
    constexpr Color old_lace()                     noexcept { return Color{253, 245, 230, 255}; }
    constexpr Color olive()                        noexcept { return Color{128, 128, 0, 255}; }
    constexpr Color olive_drab()                   noexcept { return Color{107, 142, 35, 255}; }
    constexpr Color orange()                       noexcept { return Color{255, 165, 0, 255}; }
    constexpr Color orange_red()                   noexcept { return Color{255, 69, 0, 255}; }
    constexpr Color orchid()                       noexcept { return Color{218, 112, 214, 255}; }
    constexpr Color pale_golden_rod()              noexcept { return Color{238, 232, 170, 255}; }
    constexpr Color pale_green()                   noexcept { return Color{152, 251, 152, 255}; }
    constexpr Color pale_turquoise()               noexcept { return Color{175, 238, 238, 255}; }
    constexpr Color pale_violet_red()              noexcept { return Color{219, 112, 147, 255}; }
    constexpr Color papaya_whip()                  noexcept { return Color{255, 239, 213, 255}; }
    constexpr Color peach_puff()                   noexcept { return Color{255, 218, 185, 255}; }
    constexpr Color peru()                         noexcept { return Color{205, 133, 63, 255}; }
    constexpr Color pink()                         noexcept { return Color{255, 192, 203, 255}; }
    constexpr Color plum()                         noexcept { return Color{221, 160, 221, 255}; }
    constexpr Color powder_blue()                  noexcept { return Color{176, 224, 230, 255}; }
    constexpr Color purple()                       noexcept { return Color{128, 0, 128, 255}; }
    constexpr Color rebecca_purple()               noexcept { return Color{102, 51, 153, 255}; }
    constexpr Color red()                          noexcept { return Color{255, 0, 0, 255}; }
    constexpr Color rosy_brown()                   noexcept { return Color{188, 143, 143, 255}; }
    constexpr Color royal_blue()                   noexcept { return Color{65, 105, 225, 255}; }
    constexpr Color saddle_brown()                 noexcept { return Color{139, 69, 19, 255}; }
    constexpr Color salmon()                       noexcept { return Color{250, 128, 114, 255}; }
    constexpr Color sandy_brown()                  noexcept { return Color{244, 164, 96, 255}; }
    constexpr Color sea_green()                    noexcept { return Color{46, 139, 87, 255}; }
    constexpr Color sea_shell()                    noexcept { return Color{255, 245, 238, 255}; }
    constexpr Color sienna()                       noexcept { return Color{160, 82, 45, 255}; }
    constexpr Color silver()                       noexcept { return Color{192, 192, 192, 255}; }
    constexpr Color sky_blue()                     noexcept { return Color{135, 206, 235, 255}; }
    constexpr Color slate_blue()                   noexcept { return Color{106, 90, 205, 255}; }
    constexpr Color slate_gray()                   noexcept { return Color{112, 128, 144, 255}; }
    constexpr Color slate_grey()                   noexcept { return Color{112, 128, 144, 255}; }
    constexpr Color snow()                         noexcept { return Color{255, 250, 250, 255}; }
    constexpr Color spring_green()                 noexcept { return Color{0, 255, 127, 255}; }
    constexpr Color steel_blue()                   noexcept { return Color{70, 130, 180, 255}; }
    constexpr Color tan()                          noexcept { return Color{210, 180, 140, 255}; }
    constexpr Color teal()                         noexcept { return Color{0, 128, 128, 255}; }
    constexpr Color thistle()                      noexcept { return Color{216, 191, 216, 255}; }
    constexpr Color tomato()                       noexcept { return Color{255, 99, 71, 255}; }
    constexpr Color turquoise()                    noexcept { return Color{64, 224, 208, 255}; }
    constexpr Color violet()                       noexcept { return Color{238, 130, 238, 255}; }
    constexpr Color wheat()                        noexcept { return Color{245, 222, 179, 255}; }
    constexpr Color white()                        noexcept { return Color{255, 255, 255, 255}; }
    constexpr Color white_smoke()                  noexcept { return Color{245, 245, 245, 255}; }
    constexpr Color yellow()                       noexcept { return Color{255, 255, 0, 255}; }
    constexpr Color yellow_green()                 noexcept { return Color{154, 205, 50, 255}; }
} // namespace colors
/** @} */
