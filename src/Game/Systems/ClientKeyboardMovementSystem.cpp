#include "Systems.h"

#include "../Components/AccelerationComponent.h"
#include "../Components/KeyboardMovementComponent.h"
#include "../Components/VelocityComponent.h"
#include "../settings.h"
#include "SFML/Window/Keyboard.hpp"

namespace Luntik::Systems {
void ClientKeyboardMovementSystem(entt::registry &registry, float deltaTime) {
  constexpr float speed = 10 * Settings::BLOCK_SIZE;

  registry
      .view<Components::KeyboardMovementComponent,
            Components::AccelerationComponent, Components::VelocityComponent>()
      .each([&](auto entity, Components::KeyboardMovementComponent &,
                Components::AccelerationComponent &accelerationComponent,
                Components::VelocityComponent &velocityComponent) {
        int moveDir = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
          moveDir += -1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
          moveDir += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
          velocityComponent.velocity.y = -350;
        }

        accelerationComponent.direction.x = moveDir;
      });
}
} // namespace Luntik::Systems
