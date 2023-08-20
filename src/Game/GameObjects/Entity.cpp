#include "Entity.h"

#include "World.h"
#include "entt/entity/fwd.hpp"

namespace Luntik::GameObjects {
Entity::Entity(entt::entity id, World *world)
    : m_EntityId(id), m_World(world) {}

entt::entity Entity::getHandle() const { return m_EntityId; }

bool Entity::isValid() const {
  return m_World != nullptr && m_EntityId != entt::null;
}
} // namespace Luntik::GameObjects
