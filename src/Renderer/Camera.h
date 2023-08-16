#pragma once

#include "../Utils/NonCopyable.h"
#include "SFML/Graphics.hpp"
#include "SFML/System/Vector2.hpp"

#include <cstdint>

namespace Luntik::Renderer {
class Camera : public sf::View, Utils::NonCopyable {
public:
  Camera(uint32_t height);

  float getAspectRatio() const;
  float getAspectRatio(sf::Vector2f size) const;

  sf::Vector2f getTopLeft();
  sf::Vector2f getTopRight();
  sf::Vector2f getBottomLeft();
  sf::Vector2f getBottomRight();

  void setGoal(sf::Vector2f goal);
  sf::Vector2f getGoal() const;

  void tick(float deltaTime, sf::Vector2u windowSize);

private:
  void fixAspect(sf::Vector2u windowSize);

  void moveTowards(sf::Vector2f goal, float amount, float deltaTime);

  bool m_Smooth = true;
  float m_SmoothAmount = 5.f;

  sf::Vector2f m_Goal;
  uint32_t m_Height;
};
} // namespace Luntik::Renderer
