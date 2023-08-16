#pragma once

#include "../PlayerInfo.h"
#include "ClientEntity.h"

namespace Luntik::Entities {
class ClientPlayerEntity : public ClientEntity {
public:
  ClientPlayerEntity(ID_t id, PlayerInfo *player);

  void render(Renderer::Window *window) override;
  void tick(float deltaTime) override;

private:
  PlayerInfo *m_Player;
};
} // namespace Luntik::Entities
