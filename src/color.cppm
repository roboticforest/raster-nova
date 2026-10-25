module;

#include <cstdint>

export module Color;

/** @brief Color stored as a packed 0xRRGGBBAA value. */
export class Color {
public:
    /** @brief Creates transparent black. */
    constexpr Color() noexcept = default;

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
