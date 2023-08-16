#pragma once

#include "../settings.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Entities {
struct PlayerInfo {
  ID_t socketHandle;

  sf::Vector2f pos;

  PlayerInfo(ID_t socketHandle = ID_t_MAX)
      : socketHandle(socketHandle), pos(0, 0) {}
};
} // namespace Luntik::Entities
