#pragma once

#include <cstdint>

namespace Luntik::Settings {
constexpr uint16_t BLOCK_SIZE = 16;

constexpr uint16_t CHUNK_SIZE = 5;
constexpr uint16_t CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
constexpr uint16_t CHUNK_SIZE_IN_PIXELS = CHUNK_SIZE * BLOCK_SIZE;
} // namespace Luntik::Settings
