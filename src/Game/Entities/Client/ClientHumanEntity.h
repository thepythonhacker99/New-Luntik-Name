#pragma once

#include "../../../Renderer/Window.h"
#include "../PlayerInfo.h"
#include "ClientEntity.h"

namespace Luntik::Entities {
class ClientHumanEntity : public ClientEntity {
public:
  ClientHumanEntity(ID_t id);

  void render(Renderer::Window *window) override;
};
} // namespace Luntik::Entities
