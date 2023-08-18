#pragma once

#include "Entity.h"
#include "spdlog/spdlog.h"

#include <unordered_map>

namespace Luntik::Entities {
template <typename Ent_T> class EntityManager {
public:
  EntityManager() {}

  ~EntityManager() {
    for (auto &[id, entity] : m_Entites) {
      delete entity;
    }
  }

  void tick(float deltaTime) {
    for (auto &[id, entity] : m_Entites) {
      entity->tick(deltaTime);
    }
  }

  std::unordered_map<ID_t, Ent_T> *getEntities() { return &m_Entites; }

  template <typename T> T *addEntity(ID_t id, T *entityToAdd) {
    m_Entites[id] = entityToAdd;
    return entityToAdd;
  }

  void removeEntity(ID_t id) {
    if (!entityExists(id))
      SPDLOG_INFO("Tried to remove entity with id {} but it doesn't exist!",
                  id);

    delete m_Entites.at(id);
    m_Entites.erase(id);
  }

  bool entityExists(ID_t id) { return m_Entites.find(id) != m_Entites.end(); }

  Ent_T *getEntity(ID_t id) {
    return entityExists(id) ? m_Entites.at(id) : nullptr;
  }

protected:
  std::unordered_map<ID_t, Ent_T *> m_Entites;
};
} // namespace Luntik::Entities
