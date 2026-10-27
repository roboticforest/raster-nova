/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2026 David Vitez
 * @par License:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

module;

#include <concepts>
#include <cstdint>
#include <stdexcept>

export module Color;

/** @brief Color stored as a packed 0xRRGGBBAA value. */
export class Color {
public:
    /** @brief Creates transparent black. */
    constexpr Color() noexcept = default;

    /** @brief Creates a color using from_rgba(). @param value Packed 0xRRGGBBAA color. */
    constexpr explicit Color(const std::uint32_t value) noexcept : Color(from_rgba(value)) {}

    /**
     * @brief Packs byte channels into an RGBA color.
     * @param red Red channel.
     * @param green Green channel.
     * @param blue Blue channel.
     * @param alpha Alpha channel; defaults to fully opaque and is stored without blending.
     */
    constexpr Color(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue, const std::uint8_t alpha = 255) noexcept
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
    static constexpr Color from_rgb(const std::uint32_t value) noexcept {
        return from_rgba((value << 8) | 0xFFu);
    }

    /**
     * @brief Creates a color from a packed RGBA value.
     * @param value Packed 0xRRGGBBAA color.
     * @return The color with all four channels preserved.
     */
    static constexpr Color from_rgba(const std::uint32_t value) noexcept {
        Color color;
        color.rgba(value);
        return color;
    }

    /**
     * @brief Composites this foreground color over the supplied background.
     * @param background Color underneath this color.
     * @return A new straight RGBA color; neither input is modified.
     * @note Composites stored RGB channels directly, without color-space conversion.
     *       Channels and alpha are rounded to the nearest byte value.
     */
    constexpr Color blend_over(Color background) const noexcept {
        if(alpha() == 0) { return background; }
        if(alpha() == 255 || background.alpha() == 0) { return *this; }

        // Keep the weights scaled by 255 to avoid rounding intermediate alpha values.
        const std::uint32_t foreground_weight = static_cast<std::uint32_t>(alpha()) * 255u;
        const std::uint32_t background_weight = static_cast<std::uint32_t>(background.alpha()) * (255u - alpha());
        const std::uint32_t total_weight = foreground_weight + background_weight;
        const auto blend_channel = [=](const std::uint8_t foreground_alpha, const std::uint8_t background_alpha) {
            return static_cast<std::uint8_t>((foreground_alpha * foreground_weight + background_alpha * background_weight
                                             + total_weight / 2u) / total_weight);
        };

        return Color{blend_channel(red(), background.red()),
                     blend_channel(green(), background.green()),
                     blend_channel(blue(), background.blue()),
                     static_cast<std::uint8_t>((total_weight + 127u) / 255u)};
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
    constexpr void red(const std::uint8_t value) noexcept { _rgba = (_rgba & 0x00FFFFFFu) | (static_cast<std::uint32_t>(value) << 24); }
    constexpr void green(const std::uint8_t value) noexcept { _rgba = (_rgba & 0xFF00FFFFu) | (static_cast<std::uint32_t>(value) << 16); }
    constexpr void blue(const std::uint8_t value) noexcept { _rgba = (_rgba & 0xFFFF00FFu) | (static_cast<std::uint32_t>(value) << 8); }
    constexpr void alpha(const std::uint8_t value) noexcept { _rgba = (_rgba & 0xFFFFFF00u) | static_cast<std::uint32_t>(value); }
    /** @} */

    /**
     * @name Normalized channel setters
     * @brief Replace one channel, rounding to the nearest byte and preserving the other channels.
     * @param value Normalized channel in [0.0, 1.0], mapping to [0, 255].
     * @throws std::out_of_range If value is outside [0.0, 1.0] or is NaN; the color is unchanged.
     * @{
     */
    constexpr void red(const std::floating_point auto value) { red(_normalized_to_byte(value)); }
    constexpr void green(const std::floating_point auto value) { green(_normalized_to_byte(value)); }
    constexpr void blue(const std::floating_point auto value) { blue(_normalized_to_byte(value)); }
    constexpr void alpha(const std::floating_point auto value) { alpha(_normalized_to_byte(value)); }
    /** @} */

    /** @brief Returns the packed 0xRRGGBBAA value. */
    constexpr std::uint32_t rgba() const noexcept { return _rgba; }

    /** @brief Replaces all channels with the supplied packed 0xRRGGBBAA value. @param value Packed color. */
    constexpr void rgba(const std::uint32_t value) noexcept { _rgba = value; }

private:
    static constexpr std::uint8_t _normalized_to_byte(const std::floating_point auto value) {
        if(!(value >= 0.0 && value <= 1.0)) {
            throw std::out_of_range("Color channel must be in [0.0, 1.0].");
        }
        return static_cast<std::uint8_t>(value * 255.0 + 0.5);
    }

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
    constexpr Color alice_blue()              noexcept { return Color{240, 248, 255}; }
    constexpr Color antique_white()           noexcept { return Color{250, 235, 215}; }
    constexpr Color aqua()                    noexcept { return Color{0, 255, 255}; }
    constexpr Color aquamarine()              noexcept { return Color{127, 255, 212}; }
    constexpr Color azure()                   noexcept { return Color{240, 255, 255}; }
    constexpr Color beige()                   noexcept { return Color{245, 245, 220}; }
    constexpr Color bisque()                  noexcept { return Color{255, 228, 196}; }
    constexpr Color black()                   noexcept { return Color{0, 0, 0}; }
    constexpr Color blanched_almond()         noexcept { return Color{255, 235, 205}; }
    constexpr Color blue()                    noexcept { return Color{0, 0, 255}; }
    constexpr Color blue_violet()             noexcept { return Color{138, 43, 226}; }
    constexpr Color brown()                   noexcept { return Color{165, 42, 42}; }
    constexpr Color burly_wood()              noexcept { return Color{222, 184, 135}; }
    constexpr Color cadet_blue()              noexcept { return Color{95, 158, 160}; }
    constexpr Color chartreuse()              noexcept { return Color{127, 255, 0}; }
    constexpr Color chocolate()               noexcept { return Color{210, 105, 30}; }
    constexpr Color coral()                   noexcept { return Color{255, 127, 80}; }
    constexpr Color cornflower_blue()         noexcept { return Color{100, 149, 237}; }
    constexpr Color cornsilk()                noexcept { return Color{255, 248, 220}; }
    constexpr Color crimson()                 noexcept { return Color{220, 20, 60}; }
    constexpr Color cyan()                    noexcept { return Color{0, 255, 255}; }
    constexpr Color dark_blue()               noexcept { return Color{0, 0, 139}; }
    constexpr Color dark_cyan()               noexcept { return Color{0, 139, 139}; }
    constexpr Color dark_golden_rod()         noexcept { return Color{184, 134, 11}; }
    constexpr Color dark_gray()               noexcept { return Color{169, 169, 169}; }
    constexpr Color dark_green()              noexcept { return Color{0, 100, 0}; }
    constexpr Color dark_grey()               noexcept { return Color{169, 169, 169}; }
    constexpr Color dark_khaki()              noexcept { return Color{189, 183, 107}; }
    constexpr Color dark_magenta()            noexcept { return Color{139, 0, 139}; }
    constexpr Color dark_olive_green()        noexcept { return Color{85, 107, 47}; }
    constexpr Color dark_orange()             noexcept { return Color{255, 140, 0}; }
    constexpr Color dark_orchid()             noexcept { return Color{153, 50, 204}; }
    constexpr Color dark_red()                noexcept { return Color{139, 0, 0}; }
    constexpr Color dark_salmon()             noexcept { return Color{233, 150, 122}; }
    constexpr Color dark_sea_green()          noexcept { return Color{143, 188, 143}; }
    constexpr Color dark_slate_blue()         noexcept { return Color{72, 61, 139}; }
    constexpr Color dark_slate_gray()         noexcept { return Color{47, 79, 79}; }
    constexpr Color dark_slate_grey()         noexcept { return Color{47, 79, 79}; }
    constexpr Color dark_turquoise()          noexcept { return Color{0, 206, 209}; }
    constexpr Color dark_violet()             noexcept { return Color{148, 0, 211}; }
    constexpr Color deep_pink()               noexcept { return Color{255, 20, 147}; }
    constexpr Color deep_sky_blue()           noexcept { return Color{0, 191, 255}; }
    constexpr Color dim_gray()                noexcept { return Color{105, 105, 105}; }
    constexpr Color dim_grey()                noexcept { return Color{105, 105, 105}; }
    constexpr Color dodger_blue()             noexcept { return Color{30, 144, 255}; }
    constexpr Color firebrick()               noexcept { return Color{178, 34, 34}; }
    constexpr Color floral_white()            noexcept { return Color{255, 250, 240}; }
    constexpr Color forest_green()            noexcept { return Color{34, 139, 34}; }
    constexpr Color fuchsia()                 noexcept { return Color{255, 0, 255}; }
    constexpr Color gainsboro()               noexcept { return Color{220, 220, 220}; }
    constexpr Color ghost_white()             noexcept { return Color{248, 248, 255}; }
    constexpr Color gold()                    noexcept { return Color{255, 215, 0}; }
    constexpr Color golden_rod()              noexcept { return Color{218, 165, 32}; }
    constexpr Color gray()                    noexcept { return Color{128, 128, 128}; }
    constexpr Color green()                   noexcept { return Color{0, 128, 0}; }
    constexpr Color green_yellow()            noexcept { return Color{173, 255, 47}; }
    constexpr Color grey()                    noexcept { return Color{128, 128, 128}; }
    constexpr Color honeydew()                noexcept { return Color{240, 255, 240}; }
    constexpr Color hot_pink()                noexcept { return Color{255, 105, 180}; }
    constexpr Color indian_red()              noexcept { return Color{205, 92, 92}; }
    constexpr Color indigo()                  noexcept { return Color{75, 0, 130}; }
    constexpr Color ivory()                   noexcept { return Color{255, 255, 240}; }
    constexpr Color khaki()                   noexcept { return Color{240, 230, 140}; }
    constexpr Color lavender()                noexcept { return Color{230, 230, 250}; }
    constexpr Color lavender_blush()          noexcept { return Color{255, 240, 245}; }
    constexpr Color lawn_green()              noexcept { return Color{124, 252, 0}; }
    constexpr Color lemon_chiffon()           noexcept { return Color{255, 250, 205}; }
    constexpr Color light_blue()              noexcept { return Color{173, 216, 230}; }
    constexpr Color light_coral()             noexcept { return Color{240, 128, 128}; }
    constexpr Color light_cyan()              noexcept { return Color{224, 255, 255}; }
    constexpr Color light_golden_rod_yellow() noexcept { return Color{250, 250, 210}; }
    constexpr Color light_gray()              noexcept { return Color{211, 211, 211}; }
    constexpr Color light_green()             noexcept { return Color{144, 238, 144}; }
    constexpr Color light_grey()              noexcept { return Color{211, 211, 211}; }
    constexpr Color light_pink()              noexcept { return Color{255, 182, 193}; }
    constexpr Color light_salmon()            noexcept { return Color{255, 160, 122}; }
    constexpr Color light_sea_green()         noexcept { return Color{32, 178, 170}; }
    constexpr Color light_sky_blue()          noexcept { return Color{135, 206, 250}; }
    constexpr Color light_slate_gray()        noexcept { return Color{119, 136, 153}; }
    constexpr Color light_slate_grey()        noexcept { return Color{119, 136, 153}; }
    constexpr Color light_steel_blue()        noexcept { return Color{176, 196, 222}; }
    constexpr Color light_yellow()            noexcept { return Color{255, 255, 224}; }
    constexpr Color lime()                    noexcept { return Color{0, 255, 0}; }
    constexpr Color lime_green()              noexcept { return Color{50, 205, 50}; }
    constexpr Color linen()                   noexcept { return Color{250, 240, 230}; }
    constexpr Color magenta()                 noexcept { return Color{255, 0, 255}; }
    constexpr Color maroon()                  noexcept { return Color{128, 0, 0}; }
    constexpr Color medium_aquamarine()       noexcept { return Color{102, 205, 170}; }
    constexpr Color medium_blue()             noexcept { return Color{0, 0, 205}; }
    constexpr Color medium_orchid()           noexcept { return Color{186, 85, 211}; }
    constexpr Color medium_purple()           noexcept { return Color{147, 112, 219}; }
    constexpr Color medium_sea_green()        noexcept { return Color{60, 179, 113}; }
    constexpr Color medium_slate_blue()       noexcept { return Color{123, 104, 238}; }
    constexpr Color medium_spring_green()     noexcept { return Color{0, 250, 154}; }
    constexpr Color medium_turquoise()        noexcept { return Color{72, 209, 204}; }
    constexpr Color medium_violet_red()       noexcept { return Color{199, 21, 133}; }
    constexpr Color midnight_blue()           noexcept { return Color{25, 25, 112}; }
    constexpr Color mint_cream()              noexcept { return Color{245, 255, 250}; }
    constexpr Color misty_rose()              noexcept { return Color{255, 228, 225}; }
    constexpr Color moccasin()                noexcept { return Color{255, 228, 181}; }
    constexpr Color navajo_white()            noexcept { return Color{255, 222, 173}; }
    constexpr Color navy()                    noexcept { return Color{0, 0, 128}; }
    constexpr Color old_lace()                noexcept { return Color{253, 245, 230}; }
    constexpr Color olive()                   noexcept { return Color{128, 128, 0}; }
    constexpr Color olive_drab()              noexcept { return Color{107, 142, 35}; }
    constexpr Color orange()                  noexcept { return Color{255, 165, 0}; }
    constexpr Color orange_red()              noexcept { return Color{255, 69, 0}; }
    constexpr Color orchid()                  noexcept { return Color{218, 112, 214}; }
    constexpr Color pale_golden_rod()         noexcept { return Color{238, 232, 170}; }
    constexpr Color pale_green()              noexcept { return Color{152, 251, 152}; }
    constexpr Color pale_turquoise()          noexcept { return Color{175, 238, 238}; }
    constexpr Color pale_violet_red()         noexcept { return Color{219, 112, 147}; }
    constexpr Color papaya_whip()             noexcept { return Color{255, 239, 213}; }
    constexpr Color peach_puff()              noexcept { return Color{255, 218, 185}; }
    constexpr Color peru()                    noexcept { return Color{205, 133, 63}; }
    constexpr Color pink()                    noexcept { return Color{255, 192, 203}; }
    constexpr Color plum()                    noexcept { return Color{221, 160, 221}; }
    constexpr Color powder_blue()             noexcept { return Color{176, 224, 230}; }
    constexpr Color purple()                  noexcept { return Color{128, 0, 128}; }
    constexpr Color rebecca_purple()          noexcept { return Color{102, 51, 153}; }
    constexpr Color red()                     noexcept { return Color{255, 0, 0}; }
    constexpr Color rosy_brown()              noexcept { return Color{188, 143, 143}; }
    constexpr Color royal_blue()              noexcept { return Color{65, 105, 225}; }
    constexpr Color saddle_brown()            noexcept { return Color{139, 69, 19}; }
    constexpr Color salmon()                  noexcept { return Color{250, 128, 114}; }
    constexpr Color sandy_brown()             noexcept { return Color{244, 164, 96}; }
    constexpr Color sea_green()               noexcept { return Color{46, 139, 87}; }
    constexpr Color sea_shell()               noexcept { return Color{255, 245, 238}; }
    constexpr Color sienna()                  noexcept { return Color{160, 82, 45}; }
    constexpr Color silver()                  noexcept { return Color{192, 192, 192}; }
    constexpr Color sky_blue()                noexcept { return Color{135, 206, 235}; }
    constexpr Color slate_blue()              noexcept { return Color{106, 90, 205}; }
    constexpr Color slate_gray()              noexcept { return Color{112, 128, 144}; }
    constexpr Color slate_grey()              noexcept { return Color{112, 128, 144}; }
    constexpr Color snow()                    noexcept { return Color{255, 250, 250}; }
    constexpr Color spring_green()            noexcept { return Color{0, 255, 127}; }
    constexpr Color steel_blue()              noexcept { return Color{70, 130, 180}; }
    constexpr Color tan()                     noexcept { return Color{210, 180, 140}; }
    constexpr Color teal()                    noexcept { return Color{0, 128, 128}; }
    constexpr Color thistle()                 noexcept { return Color{216, 191, 216}; }
    constexpr Color tomato()                  noexcept { return Color{255, 99, 71}; }
    constexpr Color turquoise()               noexcept { return Color{64, 224, 208}; }
    constexpr Color violet()                  noexcept { return Color{238, 130, 238}; }
    constexpr Color wheat()                   noexcept { return Color{245, 222, 179}; }
    constexpr Color white()                   noexcept { return Color{255, 255, 255}; }
    constexpr Color white_smoke()             noexcept { return Color{245, 245, 245}; }
    constexpr Color yellow()                  noexcept { return Color{255, 255, 0}; }
    constexpr Color yellow_green()            noexcept { return Color{154, 205, 50}; }
} // namespace colors
/** @} */
