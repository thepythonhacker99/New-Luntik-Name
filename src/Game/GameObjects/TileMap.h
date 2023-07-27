#pragma once

#include "../../Utils/Pos.h"
#include "../settings.h"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <unordered_map>

namespace Luntik::GameObjects {
class TileMap {
public:
  TileMap(sf::Texture *tileTexture, int tileSize);
  TileMap(sf::Texture *tileTexture, int tilesOnX, int tilesOnY);

  void registerTile(ID_t, Utils::Pos pos);

  sf::Sprite getTile(ID_t id);
  void setTexture(sf::Sprite *sprite, ID_t id);

private:
  sf::IntRect getRect(int x, int y);

  sf::Texture *m_TileMapTexture;

  int m_TileSize;
  int m_TilesOnX, m_TilesOnY;

  std::unordered_map<ID_t, Utils::Pos> m_Tiles;
};
} // namespace Luntik::GameObjects
