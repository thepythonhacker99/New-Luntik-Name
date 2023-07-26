#include "Terrain.h"

#include "../../Utils/Pos.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstdint>

namespace Luntik::GameObjects {

Terrain::Terrain() {}

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
