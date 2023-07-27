#pragma once

#include "SFML/Graphics/Texture.hpp"
#include <stdexcept>

namespace Luntik::Textures {
inline sf::Texture s_TerrainTexture;

inline sf::Texture loadTexture(const std::string &path) {
  sf::Texture texture;
  if (!texture.loadFromFile(path)) {
    throw std::runtime_error("Couldn't load texture from file: " + path);
  }

  return texture;
}

inline void loadTextures() {
  s_TerrainTexture = loadTexture("images/terrain.png");
}
} // namespace Luntik::Textures
