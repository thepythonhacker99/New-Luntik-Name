#pragma once

#include "Terrain.h"

namespace Luntik::GameObjects {
class TerrainManager : Utils::NonCopyable {
public:
  TerrainManager(Terrain *terrainToManage, Renderer::Window *window);

  void tick(float deltaTime);

private:
  void requestChunksIfNotPresent();

  Utils::Pos pixelToChunkPos(sf::Vector2f pixel);

  Utils::Pos m_TopLeftChunk;
  Utils::Pos m_BottomRightChunk;

  Terrain *m_TerrainToManage;
  Renderer::Window *m_Window;
};
} // namespace Luntik::GameObjects
