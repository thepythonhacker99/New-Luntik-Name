#include "ClientEntityManager.h"

namespace Luntik::Entities {
ClientEntityManager::ClientEntityManager() {}

ClientEntityManager::~ClientEntityManager() {}

void ClientEntityManager::render(Renderer::Window *window) {
  for (auto &[id, entity] : m_Entites) {
    entity->render(window);
  }
}
} // namespace Luntik::Entities
