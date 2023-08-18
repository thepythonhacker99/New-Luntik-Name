#pragma once

#include "ClientHumanEntity.h"
#include "ClientNetworkEntity.h"

namespace Luntik::Entities {
class ClientNetworkPlayer : public ClientNetworkEntity,
                            public ClientHumanEntity {
public:
  ClientNetworkPlayer(ID_t id);
};
} // namespace Luntik::Entities
