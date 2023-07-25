#include "Terrain.h"

#include "../../Utils/Pos.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstdint>

namespace Luntik::GameObjects {

Terrain::Terrain() { m_Terrain[Utils::Pos(0, 0)] = Chunk(Utils::Pos(0, 0)); }

std::unordered_map<Utils::Pos, Chunk> *Terrain::getTerrain() {
  return &m_Terrain;
}

Chunk *Terrain::getChunk(Utils::Pos pos) {
  return m_Terrain.find(pos) == m_Terrain.end() ? nullptr : &m_Terrain.at(pos);
}

void Terrain::render(Renderer::Window *window) {
  sf::RectangleShape rect;
  rect.setSize(sf::Vector2f{Settings::BLOCK_SIZE, Settings::BLOCK_SIZE});
  rect.setFillColor(sf::Color(30, 30, 15));

  for (const auto &[chunkPos, chunk] : m_Terrain) {
    for (const auto &[blockPos, type] : chunk.blocks) {
      if (type == BlockType::Air)
        continue;
      rect.setPosition(
          sf::Vector2f{float(Settings::CHUNK_SIZE_IN_PIXELS * chunkPos.x +
                             Settings::BLOCK_SIZE * blockPos.x),
                       float(Settings::CHUNK_SIZE_IN_PIXELS * chunkPos.y +
                             Settings::BLOCK_SIZE * blockPos.y)});

      window->getInternalWindow()->draw(rect);
    }
  }
}
} // namespace Luntik::GameObjects

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::GameObjects::BlockType &type) {
  return packet << static_cast<uint16_t>(type);
}

sf::Packet &operator>>(sf::Packet &packet,
                       Luntik::GameObjects::BlockType &type) {
  return packet >> type;
}

sf::Packet &operator<<(sf::Packet &packet,
                       const Luntik::GameObjects::Chunk &chunk) {
  packet << chunk.pos;

  for (uint16_t i = 0; i < Luntik::Settings::CHUNK_SIZE_SQUARED; i++) {
    packet << chunk.blocks[i].type;
  }

  return packet;
}

sf::Packet &operator>>(sf::Packet &packet, Luntik::GameObjects::Chunk &chunk) {
  packet >> chunk.pos;

  for (uint16_t i = 0; i < Luntik::Settings::CHUNK_SIZE_SQUARED; i++) {
    packet >> chunk.blocks[i].type;
    chunk.blocks[i].pos =
        Luntik::Utils::Pos(uint16_t(i % Luntik::Settings::CHUNK_SIZE),
                           uint16_t(i / Luntik::Settings::CHUNK_SIZE));
  }

  return packet;
}
