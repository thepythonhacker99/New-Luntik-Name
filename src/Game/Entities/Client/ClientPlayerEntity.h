#pragma once

#include "../PlayerInfo.h"
#include "ClientEntity.h"
#include "ClientHumanEntity.h"

namespace Luntik::Entities {
class ClientPlayerEntity : public ClientHumanEntity {
public:
  ClientPlayerEntity(ID_t id, PlayerInfo *player);

  void tick(float deltaTime) override;

private:
  PlayerInfo *m_Player;
};
} // namespace Luntik::Entities
