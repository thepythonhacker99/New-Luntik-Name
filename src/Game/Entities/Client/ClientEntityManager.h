#pragma once

#include "../EntityManager.h"
#include "ClientEntity.h"

namespace Luntik::Entities {
class ClientEntityManager : public EntityManager<ClientEntity> {
public:
  ClientEntityManager();
  ~ClientEntityManager();

  void render(Renderer::Window *window);

private:
};
} // namespace Luntik::Entities
