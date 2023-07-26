#pragma once

#include "SFML/Graphics/Color.hpp"

#include <cstdint>

namespace Luntik::Settings {
constexpr sf::Color CLEAR_COLOR = sf::Color(15, 15, 30);

constexpr uint16_t BLOCK_SIZE = 16;

constexpr uint16_t CHUNK_SIZE = 25;
constexpr uint16_t CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
constexpr uint16_t CHUNK_SIZE_IN_PIXELS = CHUNK_SIZE * BLOCK_SIZE;
} // namespace Luntik::Settings
