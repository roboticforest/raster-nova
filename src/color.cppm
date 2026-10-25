module;

#include <cstdint>

export module Color;

/** @brief Color stored as a packed 0xRRGGBBAA value. */
export class Color {
public:
    /** @brief Creates transparent black. */
    constexpr Color() noexcept = default;

    /**
     * @brief Packs the low eight bits of each channel into an RGBA color.
     * @param red Red channel.
     * @param green Green channel.
     * @param blue Blue channel.
     * @param alpha Alpha channel; stored without blending.
     */
    constexpr Color(int red, int green, int blue, int alpha) noexcept
        : _rgba(((static_cast<std::uint32_t>(red) & 0xFFu) << 24)
              | ((static_cast<std::uint32_t>(green) & 0xFFu) << 16)
              | ((static_cast<std::uint32_t>(blue) & 0xFFu) << 8)
              | (static_cast<std::uint32_t>(alpha) & 0xFFu)) {
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
     * @param value New channel value; only the low eight bits are used.
     * @{
     */
    constexpr void red(int value) noexcept { _rgba = (_rgba & 0x00FFFFFFu) | ((static_cast<std::uint32_t>(value) & 0xFFu) << 24); }
    constexpr void green(int value) noexcept { _rgba = (_rgba & 0xFF00FFFFu) | ((static_cast<std::uint32_t>(value) & 0xFFu) << 16); }
    constexpr void blue(int value) noexcept { _rgba = (_rgba & 0xFFFF00FFu) | ((static_cast<std::uint32_t>(value) & 0xFFu) << 8); }
    constexpr void alpha(int value) noexcept { _rgba = (_rgba & 0xFFFFFF00u) | (static_cast<std::uint32_t>(value) & 0xFFu); }
    /** @} */

    /** @brief Returns the packed 0xRRGGBBAA value. */
    constexpr std::uint32_t rgba() const noexcept { return _rgba; }

    /** @brief Replaces all channels with the supplied packed 0xRRGGBBAA value. @param value Packed color. */
    constexpr void rgba(std::uint32_t value) noexcept { _rgba = value; }

private:
    std::uint32_t _rgba = 0;
};
