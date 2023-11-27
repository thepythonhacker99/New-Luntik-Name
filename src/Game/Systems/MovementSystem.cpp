#include "Systems.h"

#include "../Components/AccelerationComponent.h"
#include "../Components/BoundingBoxComponent.h"
#include "../Components/FrictionComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/VelocityComponent.h"
#include "../GameObjects/Terrain.h"
#include "SFML/System/Vector2.hpp"
#include "spdlog/spdlog.h"
#include <cmath>

namespace Luntik::Systems {
void moveAndCollide(GameObjects::Terrain *terrain,
                    Components::PositionComponent &positionComponent,
                    sf::Vector2f targetPos,
                    Components::VelocityComponent &velocityComponent,
                    Components::BoundingBoxComponent &boundingBoxComponent) {
  // ray trace through terrain (get closest block collision)

  RayTracing::Ray ray;
  ray.origin = positionComponent.pos;
  ray.direction = targetPos - positionComponent.pos;

  // SPDLOG_INFO("RAY ORIGIN = {}, {}; RAY DIR = {}, {}; ", ray.origin.x,
  // ray.origin.y, ray.direction.x, ray.direction.y);

  GameObjects::TerrainRayTraceResult result = terrain->rayTraceClosest(
      ray, boundingBoxComponent.boundingBox.halfSize / 2.f);

  if (!result.hitBlock) {
    // SPDLOG_INFO("NO COLLISION");
    positionComponent.pos = targetPos;
    return;
  }
  // SPDLOG_INFO("COLLISION AT BLOCK {} {}", result.hitBlock->pos.x,
  // result.hitBlock->pos.y);

  // resolve
  positionComponent.pos = {
      std::lerp(positionComponent.pos.x, targetPos.x, result.t),
      std::lerp(positionComponent.pos.y, targetPos.y, result.t)};

  velocityComponent.velocity.x *= 1 - std::abs(result.normal.x);
  velocityComponent.velocity.y *= 1 - std::abs(result.normal.y);
}

void MovementSystem(entt::registry &registry, GameObjects::Terrain *terrain,
                    float deltaTime) {
  registry.view<Components::PositionComponent, Components::VelocityComponent>()
      .each([&](auto entity, Components::PositionComponent &positionComponent,
                Components::VelocityComponent &velocityComponent) {
        if (registry.all_of<Components::AccelerationComponent>(entity)) {
          Components::AccelerationComponent &accelerationComponent =
              registry.get<Components::AccelerationComponent>(entity);

          velocityComponent.velocity += accelerationComponent.speed *
                                        accelerationComponent.direction *
                                        deltaTime;
        }
        if (registry.all_of<Components::FrictionComponent>(entity)) {
          Components::FrictionComponent &frictionComponent =
              registry.get<Components::FrictionComponent>(entity);

          if (frictionComponent.applyOnX) {
            velocityComponent.velocity.x -= velocityComponent.velocity.x *
                                            frictionComponent.friction *
                                            deltaTime;
          }
          if (frictionComponent.applyOnY) {
            velocityComponent.velocity.y -= velocityComponent.velocity.y *
                                            frictionComponent.friction *
                                            deltaTime;
          }
        }

        sf::Vector2f newPos =
            positionComponent.pos + sf::Vector2f(velocityComponent) * deltaTime;

        if (registry.all_of<Components::BoundingBoxComponent>(entity)) {
          Components::BoundingBoxComponent boundingBoxComponent =
              registry.get<Components::BoundingBoxComponent>(entity);

          moveAndCollide(terrain, positionComponent, newPos, velocityComponent,
                         boundingBoxComponent);
        } else {
          positionComponent.pos = newPos;
        }
      });
}
} // namespace Luntik::Systems
