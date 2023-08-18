#include "Systems.h"

#include "../Components/PositionComponent.h"
#include "../Components/PositionInterpolatorComponent.h"
#include "../settings.h"
#include "SFML/Window/Keyboard.hpp"

namespace Luntik::Systems {
void PositionInterpolatorSystem(entt::registry &registry, float deltaTime) {
  auto view = registry.view<Components::PositionInterpolatorComponent,
                            Components::PositionComponent>();

  for (auto entity : view) {
    auto &positionComponent = view.get<Components::PositionComponent>(entity);
    auto &positionInterpolatorComponent =
        view.get<Components::PositionInterpolatorComponent>(entity);

    positionInterpolatorComponent.t += deltaTime * Settings::SERVER_TPS;
    positionComponent.pos = positionInterpolatorComponent.getPos();
  }
}
} // namespace Luntik::Systems
