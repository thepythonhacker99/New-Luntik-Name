#include "Systems.h"

#include "../Components/KeyboardMovementComponent.h"
#include "../Components/PositionComponent.h"
#include "../settings.h"
#include "SFML/Window/Keyboard.hpp"

namespace Luntik::Systems {
void ClientKeyboardMovementSystem(entt::registry &registry, float deltaTime) {
  constexpr float speed = 10 * Settings::BLOCK_SIZE;

  auto view = registry.view<Components::KeyboardMovementComponent,
                            Components::PositionComponent>();

  for (auto entity : view) {
    auto &positionComponent = view.get<Components::PositionComponent>(entity);

    sf::Vector2f moveVec;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
      moveVec.x += -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
      moveVec.x += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
      moveVec.y += -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
      moveVec.y += 1;
    }

    if (moveVec.x != 0 || moveVec.y != 0) {
      positionComponent.pos += moveVec.normalized() * speed * deltaTime;
    }
  }
}
} // namespace Luntik::Systems
