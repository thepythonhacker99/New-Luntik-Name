#pragma once

#include "../../Networking/Common.h"
#include "../../Renderer/Window.h"
#include "../../Utils/Pos.h"
#include "../settings.h"
#include "SFML/Network/Packet.hpp"

#include <cstdint>
#include <functional>
#include <iostream>
#include <unordered_map>

namespace Luntik::GameObjects {
enum BlockType { Stone, Air };

struct Block {
  Utils::Pos pos; // relative to chunk
  BlockType type;

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

  Chunk() : pos(Utils::Pos(0, 0)) {
    for (uint16_t i = 0; i < Settings::CHUNK_SIZE_SQUARED; i++) {
      blocks[i] = Block();
    }
  }

  Chunk(Utils::Pos pos) : pos(pos) {
    for (uint16_t i = 0; i < Settings::CHUNK_SIZE_SQUARED; i++) {
      blocks[i] = Block(Utils::Pos{uint16_t(i % Settings::CHUNK_SIZE),
                                   uint16_t(i / Settings::CHUNK_SIZE)},
                        BlockType::Stone);
    }
  }
};

class Terrain {
public:
  Terrain();

  void render(Renderer::Window *window);

  std::unordered_map<Utils::Pos, Chunk> *getTerrain();
  Chunk *getChunk(Utils::Pos pos);

private:
  std::unordered_map<Utils::Pos, Chunk> m_Terrain;
};
} // namespace Luntik::GameObjects

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::GameObjects::BlockType &type);

sf::Packet &operator>>(sf::Packet &packet,
                       Luntik::GameObjects::BlockType &type);

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::GameObjects::Chunk &chunk);

sf::Packet &operator>>(sf::Packet &packet, Luntik::GameObjects::Chunk &chunk);