#pragma once

#include "World.h"
#include "entt/entity/fwd.hpp"
#include <utility>

namespace Luntik::GameObjects {
class Entity {
public:
  Entity(entt::entity id, World *world);

  bool isValid() const;
  entt::entity getHandle() const;

  template <typename T, typename... Args> T &add(Args... args) {
    return m_World->m_Registry.emplace<T>(m_EntityId,
                                          std::forward<Args>(args)...);
  }

  template <typename T> T &get() {
    return m_World->m_Registry.get<T>(m_EntityId);
  }

  template <typename... Ts> bool has() {
    return m_World->m_Registry.all_of<Ts...>(m_EntityId);
  }

  template <typename... Ts> bool has_any() {
    return m_World->m_Registry.any_of<Ts...>(m_EntityId);
  }

  template <typename T> void remove() {
    m_World->m_Registry.remove<T>(m_EntityId);
  }

  operator entt::entity() { return m_EntityId; }

private:
  World *m_World;
  entt::entity m_EntityId;
};
} // namespace Luntik::GameObjects
