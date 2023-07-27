#pragma once

#include "SFML/Graphics/Color.hpp"

#include <cstddef>
#include <cstdint>

namespace Luntik {
using ID_t = size_t;
}

namespace Luntik::Settings {
constexpr sf::Color CLEAR_COLOR = sf::Color(0, 28, 48);

constexpr uint16_t BLOCK_SIZE = 16;

constexpr uint16_t CHUNK_SIZE = 25;
constexpr uint16_t CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
constexpr uint16_t CHUNK_SIZE_IN_PIXELS = CHUNK_SIZE * BLOCK_SIZE;
} // namespace Luntik::Settings
