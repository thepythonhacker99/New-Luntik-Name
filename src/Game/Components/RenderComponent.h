#pragma once

#include "SFML/Graphics/Texture.hpp"

namespace Luntik::Components {
struct RenderComponent {
  sf::Texture *texture = nullptr;
};
} // namespace Luntik::Components
