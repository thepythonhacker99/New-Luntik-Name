#include "Terrain.h"

#include "../../Utils/Pos.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"
#include "spdlog/spdlog.h"
#include <cstdint>

namespace Luntik::GameObjects {

Terrain::Terrain() { m_Noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular); }

Chunk *Terrain::generateChunk(Utils::Pos pos) {
  m_Terrain[pos] = Chunk(pos);
  Chunk *chunk = &m_Terrain.at(pos);

  for (uint16_t i = 0; i < Settings::CHUNK_SIZE_SQUARED; i++) {
    constexpr float scale = 1.f;
    constexpr float scale2 = 1.f;
    float output = m_Noise.GetNoise(float(pos.x * Settings::CHUNK_SIZE +
                                          (i % Settings::CHUNK_SIZE)) *
                                        scale,
                                    float(pos.y * Settings::CHUNK_SIZE +
                                          int(i / Settings::CHUNK_SIZE)) *
                                        scale) *
                   scale2;

    // SPDLOG_INFO("{}", output);

    if (output < 0.3) {
      chunk->blocks[i].type = BlockType::Air;
    }
  }

  return chunk;
}

std::unordered_map<Utils::Pos, Chunk> *Terrain::getTerrain() {
  return &m_Terrain;
}

Chunk *Terrain::getChunk(Utils::Pos pos) {
  return m_Terrain.find(pos) == m_Terrain.end() ? nullptr : &m_Terrain.at(pos);
}
} // namespace Luntik::GameObjects
