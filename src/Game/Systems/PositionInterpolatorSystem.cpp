#include "Systems.h"

#include "../Components/PositionComponent.h"
#include "../Components/PositionInterpolatorComponent.h"
#include "../settings.h"
#include "SFML/Window/Keyboard.hpp"
#include <algorithm>

namespace Luntik::Systems {
void PositionInterpolatorSystem(entt::registry &registry, float deltaTime) {
  registry
      .view<Components::PositionInterpolatorComponent,
            Components::PositionComponent>()
      .each([&](auto entity,
                Components::PositionInterpolatorComponent
                    &positionInterpolatorComponent,
                Components::PositionComponent &positionComponent) {
        positionInterpolatorComponent.t +=
            std::min(deltaTime * Settings::SERVER_TPS, 1.f);
        positionComponent.pos = positionInterpolatorComponent.getPos();
      });
}
} // namespace Luntik::Systems
