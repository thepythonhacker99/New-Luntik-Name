#include "Systems.h"

#include "../Components/AccelerationComponent.h"
#include "../Components/FrictionComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/VelocityComponent.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Systems {
void MovementSystem(entt::registry &registry, float deltaTime) {
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
            velocityComponent.velocity.y -=
                velocityComponent.velocity.x * frictionComponent.applyOnY *
                frictionComponent.friction * deltaTime;
          }
        }

        positionComponent.pos += sf::Vector2f(velocityComponent) * deltaTime;
      });
}
} // namespace Luntik::Systems
