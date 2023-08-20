#pragma once

#include "SFML/Graphics/Texture.hpp"

namespace Luntik::Components {
struct RenderComponent {
  sf::Texture *texture = nullptr;

  RenderComponent() = default;
  RenderComponent(const RenderComponent &) = default;
  RenderComponent(RenderComponent &) = default;
};
} // namespace Luntik::Components
