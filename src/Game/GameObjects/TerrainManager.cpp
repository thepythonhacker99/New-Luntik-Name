#include "TerrainManager.h"

#include "../Packets.h"
#include "SFML/System/Vector2.hpp"
#include "spdlog/spdlog.h"
#include <cmath>
#include <cstdint>

namespace Luntik::GameObjects {
TerrainManager::TerrainManager(Terrain *terrainToManage,
                               Networking::SocketClient *socketClient,
                               Renderer::Window *window)
    : m_TerrainToManage(terrainToManage), m_Window(window),
      m_SocketClient(socketClient) {}

Utils::Pos TerrainManager::pixelToChunkPos(sf::Vector2f pixel) {
  return Utils::Pos(std::floor(pixel.x / Settings::CHUNK_SIZE_IN_PIXELS),
                    std::floor(pixel.y / Settings::CHUNK_SIZE_IN_PIXELS));
}

void TerrainManager::onChunkReceive(const Chunk &chunk) {
  m_TerrainToManage->getTerrain()->operator[](chunk.pos) = chunk;
}

void TerrainManager::requestChunksIfNotPresent() {
  // SPDLOG_INFO("Chunks size: {} {}", m_BottomRightChunk.x - m_TopLeftChunk.x,
  // m_BottomRightChunk.y - m_TopLeftChunk.y);

  // SPDLOG_INFO("Bottom right chunk {}", m_BottomRightChunk.x,
  // m_BottomRightChunk.y);

  for (int x = m_TopLeftChunk.x - 1; x <= m_BottomRightChunk.x + 1; x++) {
    for (int y = m_TopLeftChunk.y - 1; y <= m_BottomRightChunk.y + 1; y++) {
      Utils::Pos pos(x, y);

      if (x >= m_TopLeftChunk.x && x <= m_BottomRightChunk.x) {
        if (y >= m_TopLeftChunk.y && y <= m_BottomRightChunk.y) {
          m_RenderPos.push_back(pos);
        }
      }

      if (m_TerrainToManage->getChunk(pos) != nullptr)
        continue;

      // request the chunk
      // SPDLOG_INFO("Requested chunk {} {}", pos.x, pos.y);
      m_SocketClient->send(
          Networking::createPacket<Packets::C2S_CHUNK_PACKET>(pos));
    }
  }
}

void TerrainManager::render(Renderer::Window *window) {
  sf::RectangleShape rect;
  rect.setSize(sf::Vector2f{Settings::BLOCK_SIZE, Settings::BLOCK_SIZE});
  rect.setFillColor(sf::Color(30, 30, 15));

  for (const Utils::Pos pos : m_RenderPos) {
    Chunk *chunk = m_TerrainToManage->getChunk(pos);
    if (chunk) {
      for (const auto &[blockPos, type] : chunk->blocks) {
        if (type == BlockType::Air)
          continue;
        rect.setPosition(
            sf::Vector2f{float(Settings::CHUNK_SIZE_IN_PIXELS * pos.x +
                               Settings::BLOCK_SIZE * blockPos.x),
                         float(Settings::CHUNK_SIZE_IN_PIXELS * pos.y +
                               Settings::BLOCK_SIZE * blockPos.y)});

        window->getInternalWindow()->draw(rect);
      }
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

  m_RenderPos.clear();
  requestChunksIfNotPresent();
}
} // namespace Luntik::GameObjects
