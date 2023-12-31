#include "TerrainManager.h"

#include "../Packets.h"
#include "../Textures.h"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/System/Vector2.hpp"
#include "Terrain.h"
#include "spdlog/spdlog.h"

#include <cmath>
#include <cstdint>
#include <unordered_map>

namespace Luntik::GameObjects {
TerrainManager::TerrainManager(Terrain *terrainToManage,
                               Networking::SocketClient *socketClient,
                               Renderer::Window *window)
    : m_TerrainToManage(terrainToManage), m_Window(window),
      m_TileMap(&Textures::s_TerrainTexture, Settings::BLOCK_SIZE),
      m_SocketClient(socketClient) {
  m_TileMap.registerTile(BlockType::Stone, Utils::Pos(1, 1));
}

TerrainManager::~TerrainManager() {
  m_RenderCache.clear();
  m_RenderPos.clear();
  m_RequestedChunks.clear();
}

void TerrainManager::onChunkReceive(const Chunk &chunk) {
  m_TerrainToManage->getTerrain()->emplace(chunk.pos, chunk);
  updateChunkRenderCache(chunk.pos);
}

void TerrainManager::updateChunkRenderCache(Utils::Pos pos) {
  Chunk *chunk = m_TerrainToManage->getChunk(pos);
  if (!chunk)
    return;

  if (m_RenderCache.find(pos) == m_RenderCache.end()) {
    if (m_RenderCache[pos].create(sf::Vector2u(
            Settings::CHUNK_SIZE_IN_PIXELS, Settings::CHUNK_SIZE_IN_PIXELS)) ==
        false) {
      SPDLOG_ERROR("Failed to create chunk render cache texture for pos {} {}",
                   pos.x, pos.y);
    }
  }

  sf::RenderTexture *texture = &m_RenderCache.at(pos);
  texture->clear(sf::Color::Transparent);

  for (const Block &block : chunk->blocks) {
    if (block.type == BlockType::Air)
      continue;

    sf::Sprite sprite = m_TileMap.getTile(block.type);
    sprite.setPosition(sf::Vector2f(block.pos.x * Settings::BLOCK_SIZE,
                                    block.pos.y * Settings::BLOCK_SIZE));

    texture->draw(sprite);
  }
};

void TerrainManager::requestChunksIfNotPresent() {
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

      if (std::find(m_RequestedChunks.begin(), m_RequestedChunks.end(), pos) !=
          m_RequestedChunks.end())
        continue;

      // request the chunk
      // SPDLOG_INFO("Requested chunk {} {}", pos.x, pos.y);
      m_SocketClient->send(
          Networking::createPacket<Packets::C2S_CHUNK_PACKET>(pos));

      m_RequestedChunks.push_back(pos);
    }
  }
}

void TerrainManager::render(Renderer::Window *window) {
  sf::RectangleShape rect;
  rect.setSize(sf::Vector2f{Settings::BLOCK_SIZE, Settings::BLOCK_SIZE});
  rect.setFillColor(sf::Color(30, 30, 15));

  for (const Utils::Pos pos : m_RenderPos) {
    if (m_TerrainToManage->getChunk(pos) == nullptr)
      continue;

    if (m_RenderCache.find(pos) == m_RenderCache.end()) {
      updateChunkRenderCache(pos);
    }

    sf::Sprite sprite(m_RenderCache.at(pos).getTexture());
    sprite.setPosition(sf::Vector2f(pos.x * Settings::CHUNK_SIZE_IN_PIXELS,
                                    pos.y * Settings::CHUNK_SIZE_IN_PIXELS));

    m_Window->getInternalWindow()->draw(sprite);
  }
}

void TerrainManager::tick(float deltaTime) {
  m_ClearRequestedChunks += deltaTime;

  if (m_ClearRequestedChunks >= 3.f) {
    m_ClearRequestedChunks -= 3.f;
    m_RequestedChunks.clear();
  } else {
    m_RequestedChunks.erase(
        std::remove_if(m_RequestedChunks.begin(), m_RequestedChunks.end(),
                       [this](const Utils::Pos &pos) {
                         return m_TerrainToManage->getChunk(pos) != nullptr;
                       }),
        m_RequestedChunks.end());
  }

  Utils::Pos topLeftChunk =
      Terrain::pixelToChunkPos(m_Window->getCamera()->getTopLeft());

  Utils::Pos bottomRightChunk =
      Terrain::pixelToChunkPos(m_Window->getCamera()->getBottomRight());

  if (m_TopLeftChunk == topLeftChunk && m_BottomRightChunk == bottomRightChunk)
    return;

  m_TopLeftChunk = topLeftChunk;
  m_BottomRightChunk = bottomRightChunk;

  m_RenderPos.clear();
  requestChunksIfNotPresent();

  std::erase_if(m_RenderCache, [this](const auto &pair) {
    Utils::Pos pos = pair.first;
    if (pos.x >= m_TopLeftChunk.x && pos.x <= m_BottomRightChunk.x) {
      if (pos.y >= m_TopLeftChunk.y && pos.y <= m_BottomRightChunk.y) {
        return false;
      }
    }

    return true;
  });
}
} // namespace Luntik::GameObjects
