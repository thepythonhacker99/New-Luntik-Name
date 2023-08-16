#include "Terrain.h"

#include "../../Utils/Pos.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"
#include "spdlog/spdlog.h"
#include <cstdint>
#include <ctime>

namespace Luntik::GameObjects {

Terrain::Terrain() {
  m_Noise.SetSeed(time(NULL));
  m_Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

Chunk *Terrain::generateChunk(Utils::Pos pos) {
  m_Terrain[pos] = Chunk(pos);
  Chunk *chunk = &m_Terrain.at(pos);

  for (uint16_t i = 0; i < Settings::CHUNK_SIZE_SQUARED; i++) {
    constexpr float scale = 10.f;
    float output = m_Noise.GetNoise(
        float(pos.x * Settings::CHUNK_SIZE + (i % Settings::CHUNK_SIZE)) *
            scale,
        float(pos.y * Settings::CHUNK_SIZE + (i / Settings::CHUNK_SIZE)) *
            scale);

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

Chunk *Terrain::getChunkOrGenerate(Utils::Pos pos) {
  if (m_Terrain.find(pos) == m_Terrain.end()) {
    generateChunk(pos);
  }

  return &m_Terrain.at(pos);
}

Block *Terrain::getBlock(Utils::Pos pos) {
  // SPDLOG_INFO("{} {}", pos.x, pos.y);
  // Utils::Pos chunkPos = pos / Settings::CHUNK_SIZE;
  Utils::Pos chunkPos = Utils::Pos(0, 0) / Settings::CHUNK_SIZE;

  Chunk *chunk = getChunk(chunkPos);
  if (chunk) {
    return chunk->getBlock(pos - (chunkPos * Settings::CHUNK_SIZE));
  }

  return nullptr;
}
} // namespace Luntik::GameObjects
