#include "Systems.h"

#include "../Components/GravityComponent.h"
#include "../Components/VelocityComponent.h"
#include <algorithm>

namespace Luntik::Systems {
void GravitySystem(entt::registry &registry, float deltaTime) {
  registry.view<Components::VelocityComponent, Components::GravityComponent>()
      .each([&](auto entity, Components::VelocityComponent &velocityComponent,
                Components::GravityComponent &gravityComponent) {
        velocityComponent.velocity.y += gravityComponent.gravity * deltaTime;
        velocityComponent.velocity.y =
            std::min(velocityComponent.velocity.y, gravityComponent.maxGravity);
      });
}
} // namespace Luntik::Systems
