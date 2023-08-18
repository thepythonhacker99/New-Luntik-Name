#include "ClientNetworkPlayer.h"

#include "ClientHumanEntity.h"
#include "ClientNetworkEntity.h"

namespace Luntik::Entities {
ClientNetworkPlayer::ClientNetworkPlayer(ID_t id)
    : ClientNetworkEntity(id), ClientHumanEntity(id) {}
} // namespace Luntik::Entities
