#pragma once

#include "../../Renderer/Window.h"
#include "../GameObjects/Terrain.h"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"

namespace Luntik::Systems {
void ClientKeyboardMovementSystem(entt::registry &, float dt);
void ClientRenderSystem(entt::registry &, Renderer::Window *);

void PositionInterpolatorSystem(entt::registry &, float dt);
void MovementSystem(entt::registry &, GameObjects::Terrain *terrain, float dt);
void GravitySystem(entt::registry &, float dt);
} // namespace Luntik::Systems
