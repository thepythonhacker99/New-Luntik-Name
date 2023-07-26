#pragma once

#include "../../Networking/SocketClient.h"
#include "SFML/Graphics/RenderTexture.hpp"
#include "Terrain.h"
#include <unordered_map>
#include <vector>

namespace Luntik::GameObjects {
class TerrainManager : Utils::NonCopyable {
public:
  TerrainManager(Terrain *terrainToManage,
                 Networking::SocketClient *socketClient,
                 Renderer::Window *window);
  void onChunkReceive(const Chunk &chunk);

  void tick(float deltaTime);
  void render(Renderer::Window *window);

private:
  void requestChunksIfNotPresent();
  void updateChunkRenderCache(Utils::Pos pos);

  Utils::Pos pixelToChunkPos(sf::Vector2f pixel);

  Utils::Pos m_TopLeftChunk;
  Utils::Pos m_BottomRightChunk;

  Networking::SocketClient *m_SocketClient;

  std::vector<Utils::Pos> m_RenderPos;
  std::unordered_map<Utils::Pos, sf::RenderTexture> m_RenderCache;

  Terrain *m_TerrainToManage;
  Renderer::Window *m_Window;
};
} // namespace Luntik::GameObjects
