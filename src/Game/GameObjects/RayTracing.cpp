#include "RayTracing.h"

#include "SFML/System/Vector2.hpp"

namespace Luntik::RayTracing {
RayTraceResult rayVsBoundingBox(Ray ray, GameObjects::BoundingBox box,
                                sf::Vector2f pos) {
  RayTraceResult result;

  sf::Vector2f invRayDir =
      sf::Vector2f(1.0f / ray.direction.x, 1.0f / ray.direction.y);

  float t1 = (box.getLeft() + pos.x - ray.origin.x) * invRayDir.x;
  float t2 = (box.getRight() + pos.x - ray.origin.x) * invRayDir.x;
  float t3 = (box.getTop() + pos.y - ray.origin.y) * invRayDir.y;
  float t4 = (box.getBottom() + pos.y - ray.origin.y) * invRayDir.y;

  float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
  float tmax = std::min(std::max(t1, t2), std::max(t3, t4));

  if (tmin > tmax || tmin < 0.f || tmin > 1.f) {
    return result; // No intersection
  }
  result.hasCollision = true;
  result.t = tmin;

  if (t1 == tmin) {
    result.normal = {-1.0f, 0.0f};
  } else if (t2 == tmin) {
    result.normal = {1.0f, 0.0f};
  } else if (t3 == tmin) {
    result.normal = {0.0f, -1.0f};
  } else {
    result.normal = {0.0f, 1.0f};
  }

  return result;
}
} // namespace Luntik::RayTracing
