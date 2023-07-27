#include "TileMap.h"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include "spdlog/spdlog.h"
#include <stdexcept>

namespace Luntik::GameObjects {
TileMap::TileMap(sf::Texture *tileTexture, int tileSize) {
  m_TileMapTexture = tileTexture;

  m_TileSize = tileSize;

  m_TilesOnX = m_TileMapTexture->getSize().x / m_TileSize;
  m_TilesOnY = m_TileMapTexture->getSize().y / m_TileSize;

  if (m_TilesOnX * m_TileSize != m_TileMapTexture->getSize().x) {
    SPDLOG_WARN("TileSize * TilesOnX isn't a multiple of TileMapSizeX");
  }
  if (m_TilesOnY * m_TileSize != m_TileMapTexture->getSize().y) {
    SPDLOG_WARN("TileSize * TilesOnY isn't a multiple of TileMapSizeY");
  }
}

TileMap::TileMap(sf::Texture *tileTextre, int tilesOnX, int tilesOnY) {
  m_TileMapTexture = tileTextre;

  m_TilesOnX = tilesOnX;
  m_TilesOnY = tilesOnY;

  m_TileSize = m_TileMapTexture->getSize().x / m_TilesOnX;

  if (m_TilesOnX * m_TileSize != m_TileMapTexture->getSize().x) {
    SPDLOG_WARN("TileSize * TilesOnX isn't a multiple of TileMapSizeX");
  }
  if (m_TilesOnY * m_TileSize != m_TileMapTexture->getSize().y) {
    SPDLOG_WARN("TileSize * TilesOnY isn't a multiple of TileMapSizeY");
  }
}

void TileMap::registerTile(ID_t id, Utils::Pos pos) {
  if (m_Tiles.find(id) != m_Tiles.end()) {
    SPDLOG_INFO("Warning: overriding tile");
  }

  m_Tiles[id] = pos;
}

sf::IntRect TileMap::getRect(int x, int y) {
  return sf::IntRect(sf::Vector2i(x * m_TileSize, y * m_TileSize),
                     sf::Vector2i(m_TileSize, m_TileSize));
}

sf::Sprite TileMap::getTile(ID_t id) {
  if (m_Tiles.find(id) == m_Tiles.end()) {
    throw std::runtime_error("[getTile] tile not found " + std::to_string(id));
  }

  Utils::Pos pos = m_Tiles.at(id);

  sf::Sprite sprite(*m_TileMapTexture, getRect(pos.x, pos.y));

  return std::move(sprite);
}

void TileMap::setTexture(sf::Sprite *sprite, ID_t id) {
  if (m_Tiles.find(id) == m_Tiles.end()) {
    throw std::runtime_error("[getTile] tile not found " + std::to_string(id));
  }

  Utils::Pos pos = m_Tiles.at(id);

  sprite->setTexture(*m_TileMapTexture);
  sprite->setTextureRect(getRect(pos.x, pos.y));
}
} // namespace Luntik::GameObjects
