#pragma once

#include "../../Renderer/Window.h"
#include "../../Utils/Pos.h"
#include "../settings.h"
#include "BoundingBox.h"
#include "Noise/FastNoiseLite.h"
#include "RayTracing.h"
#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"

#include <cstdint>
#include <functional>
#include <iostream>
#include <unordered_map>

namespace Luntik::GameObjects {
enum BlockType : uint8_t { Stone, Air };

struct Chunk;

struct Block {
  Chunk *chunk = nullptr;
  Utils::Pos pos; // relative to chunk
  BlockType type;

  BoundingBox getBoundingBox() const {
    return BoundingBox(
        sf::Vector2f(Settings::BLOCK_SIZE_HALF, Settings::BLOCK_SIZE_HALF));
  }

  sf::Vector2f posInPixels() const {
    return sf::Vector2f(pos.x * Settings::BLOCK_SIZE,
                        pos.y * Settings::BLOCK_SIZE);
  }

  Block() {
    pos = Utils::Pos{0, 0};
    type = BlockType::Air;
  }

  Block(Utils::Pos pos, BlockType type) {
    this->pos = pos;
    this->type = type;
  }
};

struct Chunk {
  Utils::Pos pos;
  Block blocks[Settings::CHUNK_SIZE_SQUARED];

  Chunk(BlockType fillType = Air) : pos(Utils::Pos(0, 0)) { fill(fillType); }

  Chunk(Utils::Pos pos, BlockType fillType = Air) : pos(pos) { fill(fillType); }

  sf::Vector2f posInPixels() const {
    return sf::Vector2f(pos.x * Settings::CHUNK_SIZE_IN_PIXELS,
                        pos.y * Settings::CHUNK_SIZE_IN_PIXELS);
  }

  void fill(BlockType type) {
    for (uint16_t i = 0; i < Settings::CHUNK_SIZE_SQUARED; i++) {
      blocks[i] = Block(Utils::Pos{uint16_t(i % Settings::CHUNK_SIZE),
                                   uint16_t(i / Settings::CHUNK_SIZE)},
                        type);
      blocks[i].chunk = this;
    }
  }

  Block *getBlock(Utils::Pos pos) {
    return &blocks[pos.y * Settings::CHUNK_SIZE + pos.x];
  }
};

struct TerrainRayTraceResult {
  Block *hitBlock;

  sf::Vector2f normal;
  float t;
};

class Terrain {
public:
  Terrain();

  std::unordered_map<Utils::Pos, Chunk> *getTerrain();

  Chunk *getChunk(Utils::Pos pos);
  Chunk *getChunkOrGenerate(Utils::Pos pos);

  Block *getBlock(Utils::Pos pos);

  Chunk *generateChunk(Utils::Pos pos);

  std::vector<Block *> getNearbyBlocks(Utils::Pos pos, int radius,
                                       bool includeAir = false);

  TerrainRayTraceResult
  rayTraceClosest(RayTracing::Ray ray,
                  sf::Vector2f expandBlockBoundingBoxBy = sf::Vector2f{0, 0});

  static Utils::Pos pixelToChunkPos(sf::Vector2f pixel) {
    return Utils::Pos(std::floor(pixel.x / Settings::CHUNK_SIZE_IN_PIXELS),
                      std::floor(pixel.y / Settings::CHUNK_SIZE_IN_PIXELS));
  }

  static Utils::Pos pixelToBlockPos(sf::Vector2f pixel) {
    return Utils::Pos(std::floor(pixel.x / Settings::BLOCK_SIZE),
                      std::floor(pixel.y / Settings::BLOCK_SIZE));
  }

  static Utils::Pos blockPosToChunkPos(Utils::Pos pos) {
    return Utils::Pos(
        std::floor(pos.x / static_cast<float>(Settings::CHUNK_SIZE)),
        std::floor(pos.y / static_cast<float>(Settings::CHUNK_SIZE)));
  }

private:
  std::unordered_map<Utils::Pos, Chunk> m_Terrain;
  FastNoiseLite m_Noise;
};
} // namespace Luntik::GameObjects
