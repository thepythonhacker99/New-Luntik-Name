#include "TerrainManager.h"

#include "SFML/System/Vector2.hpp"
#include "spdlog/spdlog.h"
#include <cmath>

namespace Luntik::GameObjects {
TerrainManager::TerrainManager(Terrain *terrainToManage,
                               Renderer::Window *window)
    : m_TerrainToManage(terrainToManage), m_Window(window) {}

Utils::Pos TerrainManager::pixelToChunkPos(sf::Vector2f pixel) {
  return Utils::Pos(std::floor(pixel.x / Settings::CHUNK_SIZE_IN_PIXELS),
                    std::floor(pixel.y / Settings::CHUNK_SIZE_IN_PIXELS));
}

void TerrainManager::requestChunksIfNotPresent() {
  // SPDLOG_INFO("Chunks size: {} {}", m_BottomRightChunk.x - m_TopLeftChunk.x,
  // m_BottomRightChunk.y - m_TopLeftChunk.y);

  // SPDLOG_INFO("Bottom right chunk {}", m_BottomRightChunk.x,
  // m_BottomRightChunk.y);

  for (int x = m_TopLeftChunk.x - 1; x <= m_BottomRightChunk.x + 1; x++) {
    for (int y = m_TopLeftChunk.y - 1; y <= m_BottomRightChunk.y + 1; y++) {
      Utils::Pos pos(x, y);
      if (m_TerrainToManage->getChunk(pos) != nullptr)
        continue;

      // request the chunk
      // SPDLOG_INFO("Requested chunk {} {}", pos.x, pos.y);
    }
  }
}

void TerrainManager::tick(float deltaTime) {
  Utils::Pos topLeftChunk =
      pixelToChunkPos(m_Window->getCamera()->getTopLeft());

  Utils::Pos bottomRightChunk =
      pixelToChunkPos(m_Window->getCamera()->getBottomRight());

  if (m_TopLeftChunk == topLeftChunk && m_BottomRightChunk == bottomRightChunk)
    return;

  m_TopLeftChunk = topLeftChunk;
  m_BottomRightChunk = bottomRightChunk;

  requestChunksIfNotPresent();
}
} // namespace Luntik::GameObjects
