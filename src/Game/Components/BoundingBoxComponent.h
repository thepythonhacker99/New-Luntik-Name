#pragma once

#include "../GameObjects/BoundingBox.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Components {
struct BoundingBoxComponent {
  GameObjects::BoundingBox boundingBox;

  BoundingBoxComponent() = default;
  BoundingBoxComponent(const BoundingBoxComponent &) = default;
  BoundingBoxComponent(BoundingBoxComponent &) = default;

  BoundingBoxComponent(GameObjects::BoundingBox box) : boundingBox(box) {}

  operator const GameObjects::BoundingBox &() const { return boundingBox; }
  operator GameObjects::BoundingBox &() { return boundingBox; }
};
} // namespace Luntik::Components
