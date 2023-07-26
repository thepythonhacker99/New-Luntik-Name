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
} // namespace Luntik::GameObjects
