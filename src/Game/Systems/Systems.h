#pragma once

#include "../../Renderer/Window.h"
#include "entt/entt.hpp"

namespace Luntik::Systems {
void ClientKeyboardMovementSystem(entt::registry &, float);
void ClientRenderSystem(entt::registry &, Renderer::Window *);

void PositionInterpolatorSystem(entt::registry &, float);
} // namespace Luntik::Systems
