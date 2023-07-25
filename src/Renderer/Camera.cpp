#include "Camera.h"

#include "../Utils/Math.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Renderer {
Camera::Camera(uint32_t height) : m_Height(height) {
  setCenter(sf::Vector2f(0, 0));
}

float Camera::getAspectRatio() const { return getSize().x / getSize().y; }
float Camera::getAspectRatio(sf::Vector2f size) const {
  return size.x / size.y;
}

sf::Vector2f Camera::getTopLeft() {
  return getCenter() + sf::Vector2f(-getSize().x / 2.f, -getSize().y / 2.f);
}

sf::Vector2f Camera::getTopRight() {
  return getCenter() + sf::Vector2f(getSize().x / 2.f, -getSize().y / 2.f);
}

sf::Vector2f Camera::getBottomLeft() {
  return getCenter() + sf::Vector2f(-getSize().x / 2.f, getSize().y / 2.f);
}

sf::Vector2f Camera::getBottomRight() {
  return getCenter() + sf::Vector2f(getSize().x / 2.f, getSize().y / 2.f);
}

void Camera::tick(float deltaTime, sf::Vector2u windowSize) {
  fixAspect(windowSize);
  moveTowards(m_Goal, m_SmoothAmount, deltaTime);
}

void Camera::fixAspect(sf::Vector2u windowSize) {
  setSize(sf::Vector2f(getAspectRatio(sf::Vector2f(windowSize)) * m_Height,
                       m_Height));
}

void Camera::moveTowards(sf::Vector2f goal, float amount, float deltaTime) {
  setCenter(
      sf::Vector2f(Utils::lerp(getCenter().x, goal.x, amount * deltaTime),
                   Utils::lerp(getCenter().y, goal.y, amount * deltaTime)));
}
} // namespace Luntik::Renderer
