#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstdint>
#include <stdexcept>

namespace Luntik::Textures {
// enum TerrainTileId { TERRAIN_TILE_MIDDLE, TERRAIN_TILE_TOP_LEFT };

inline sf::Texture s_TerrainTexture;
// inline sf::Texture s_EmptyTexture;

inline sf::Texture loadTexture(const std::string &path) {
  sf::Texture texture;
  if (!texture.loadFromFile(path)) {
    throw std::runtime_error("Couldn't load texture from file: " + path);
  }

  return texture;
}

inline void loadTextures() {
  // if (!s_EmptyTexture.create(sf::Vector2u(1, 1))) {
  //   throw std::runtime_error("Failed to create empty texture");
  // }
  //
  // uint8_t pixel[4] = {0, 0, 0, 0};
  // s_EmptyTexture.update(pixel);

  s_TerrainTexture = loadTexture("images/terrain.png");
}
} // namespace Luntik::Textures
