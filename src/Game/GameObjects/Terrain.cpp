#include "Terrain.h"

#include "../../Utils/Math.h"
#include "../../Utils/Pos.h"
#include "RayTracing.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"
#include "spdlog/spdlog.h"
#include <cstdint>
#include <ctime>
#include <limits>

namespace Luntik::GameObjects {

Terrain::Terrain() {
  m_Noise.SetSeed(time(NULL));
  m_Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

Chunk *Terrain::generateChunk(Utils::Pos pos) {
  m_Terrain[pos] = Chunk(pos);
  Chunk *chunk = &m_Terrain.at(pos);

  if (pos.x == -1) {
    chunk->fill(Stone);
  }

  return chunk;

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
  Utils::Pos chunkPos = blockPosToChunkPos(pos);
  // Utils::Pos chunkPos = Utils::Pos(0, 0) / Settings::CHUNK_SIZE;

  // SPDLOG_INFO("CHUNKS {}", m_Terrain.size());
  // SPDLOG_INFO("CHUNK EXISTS {}",
  // m_Terrain.find(Utils::Pos(0, 0)) != m_Terrain.end());
  // SPDLOG_INFO("CHUNK POS: {} {}", chunkPos.x, chunkPos.y);

  Chunk *chunk = getChunk(chunkPos);
  // Chunk *chunk = getChunk(Utils::Pos(0, 0));
  // SPDLOG_INFO("POS {} {}", pos.x, pos.y);
  // SPDLOG_INFO("CHUNK {} {}", chunkPos.x, chunkPos.y);
  if (chunk) {
    // SPDLOG_INFO("CHUMK EXISTS");
    return chunk->getBlock(pos - (chunkPos * Settings::CHUNK_SIZE));
  }
  // SPDLOG_INFO("CHUNK NON CREINGEIEGNI");

  return nullptr;
}

std::vector<Block *> Terrain::getNearbyBlocks(Utils::Pos pos, int radius,
                                              bool includeAir) {
  // SPDLOG_INFO("BLOCK AT {} {} is {}", pos.x, pos.y, getBlock(pos) !=
  // nullptr);

  std::vector<Block *> blocks;

  for (int y = pos.y - radius; y <= pos.y + radius; y++) {
    for (int x = pos.x - radius; x <= pos.x + radius; x++) {
      Block *block = getBlock(Utils::Pos(x, y));
      // SPDLOG_INFO("BLOCK {}", (size_t)block);
      if (block) {
        if (includeAir || block->type != Air)
          blocks.push_back(block);
      }
    }
  }

  return std::move(blocks);
}

TerrainRayTraceResult
Terrain::rayTraceClosest(RayTracing::Ray ray,
                         sf::Vector2f expandBlockBoundingBoxBy) {
  TerrainRayTraceResult nearest;

  nearest.t = std::numeric_limits<float>().max();
  nearest.hitBlock = nullptr;

  std::vector<Block *> blocks = getNearbyBlocks(pixelToBlockPos(ray.origin),
                                                ray.direction.lengthSq() + 1);

  // return nearest;
  SPDLOG_INFO("FOUND {} NEARBY BLOCKS", blocks.size());
  SPDLOG_INFO("PLAYER POS {} {}", ray.origin.x, ray.origin.y);
  SPDLOG_INFO("LENGHSQ {}", ray.direction.lengthSq() + 1);

  for (Block *block : blocks) {
    // SPDLOG_INFO("BLOCK WORLD POS: {} {}",
    //             block->posInPixels().x + block->chunk->posInPixels().x,
    //             block->posInPixels().y + block->chunk->posInPixels().y);
    SPDLOG_INFO("CHUNKJ POS {} {} ", block->chunk->pos.x, block->chunk->pos.y);
    RayTracing::RayTraceResult result = RayTracing::rayVsBoundingBox(
        ray, block->getBoundingBox().expanded(expandBlockBoundingBoxBy),
        block->posInPixels() + block->chunk->posInPixels());

    if (result.hasCollision && result.t < nearest.t) {
      SPDLOG_INFO("COLLISION");
      nearest.t = result.t;
      nearest.normal = result.normal;
      nearest.hitBlock = block;
    }
  }

  return nearest;
}
} // namespace Luntik::GameObjects
