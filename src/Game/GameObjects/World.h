#pragma once

#include "../Components/UUIDComponent.h"
#include "Terrain.h"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"
#include <unordered_map>

namespace Luntik::GameObjects {
class Entity;

class World {
public:
  World();
  ~World();

  Entity toEntity(entt::entity handleId);

  Entity create();
  void destroy(entt::entity e);

  Entity getEntityByUUID(ID_t uuid);

  Terrain *getTerrain();
  entt::registry *getRegistry();

  operator entt::registry &() { return m_Registry; }

private:
  void onUUIDComponentCreate(entt::registry &registry, entt::entity entityId);
  void onUUIDComponentDestroy(entt::registry &registry, entt::entity entityId);

  Terrain m_Terrain;
  std::unordered_map<ID_t, entt::entity> m_IdLookup;

  entt::registry m_Registry;

  friend class Entity;
};
} // namespace Luntik::GameObjects
