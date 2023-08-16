#pragma once

#include "SFML/Graphics/Color.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>

namespace Luntik {
using ID_t = size_t;
constexpr ID_t ID_t_MAX = std::numeric_limits<ID_t>().max();
} // namespace Luntik

namespace Luntik::Settings {
constexpr sf::Color CLEAR_COLOR = sf::Color(0, 28, 48);

constexpr uint16_t BLOCK_SIZE = 16;
constexpr uint16_t BLOCK_SIZE_HALF = BLOCK_SIZE / 2;

constexpr uint16_t CHUNK_SIZE = 25;
constexpr uint16_t CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
constexpr uint16_t CHUNK_SIZE_IN_PIXELS = CHUNK_SIZE * BLOCK_SIZE;

constexpr int SERVER_TPS = 20;
constexpr float MAX_POS_DIFFERENCE = 5.f;
} // namespace Luntik::Settings
