#pragma once

namespace Luntik::Components {
struct GravityComponent {
  float gravity = 1000;
  float maxGravity = 500;

  GravityComponent() = default;
  GravityComponent(const GravityComponent &) = default;
  GravityComponent(GravityComponent &) = default;

  GravityComponent(float g) : gravity(g) {}
};
} // namespace Luntik::Components
