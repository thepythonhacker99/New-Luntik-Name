#include "World.h"
#include "../Components/UUIDComponent.h"
#include "Entity.h"
#include "Terrain.h"
#include "entt/entity/fwd.hpp"
#include "spdlog/spdlog.h"

namespace Luntik::GameObjects {
World::World() {
  m_Registry.on_construct<Components::UUIDComponent>()
      .connect<&World::onUUIDComponentCreate>(this);
  m_Registry.on_destroy<Components::UUIDComponent>()
      .connect<&World::onUUIDComponentDestroy>(this);
}

World::~World() {}

void World::onUUIDComponentCreate(entt::registry &registry,
                                  entt::entity entityId) {
  Entity entity = toEntity(entityId);
  const auto &uuidComponent = entity.get<Components::UUIDComponent>();

  m_IdLookup[uuidComponent.uuid] = entityId;
}

void World::onUUIDComponentDestroy(entt::registry &registry,
                                   entt::entity entityId) {
  Entity entity = toEntity(entityId);
  const auto &uuidComponent = entity.get<Components::UUIDComponent>();

  m_IdLookup.erase(uuidComponent.uuid);
}

Entity World::getEntityByUUID(ID_t uuid) {
  if (m_IdLookup.find(uuid) != m_IdLookup.end()) {
    return Entity{m_IdLookup.at(uuid), this};
  }

  return Entity{entt::null, nullptr};
}

Entity World::toEntity(entt::entity handleId) { return Entity{handleId, this}; }

Entity World::create() { return {m_Registry.create(), this}; }
void World::destroy(entt::entity e) { m_Registry.destroy(e); }

Terrain *World::getTerrain() { return &m_Terrain; }
entt::registry *World::getRegistry() { return &m_Registry; };
} // namespace Luntik::GameObjects
