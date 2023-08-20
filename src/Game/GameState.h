#pragma once

#include "Entities/PlayerInfo.h"
#include "GameObjects/Terrain.h"
#include "GameObjects/World.h"
#include "SFML/System/Vector2.hpp"

#include <unordered_map>

namespace Luntik {
struct GameState {
  GameObjects::World world;

  std::unordered_map<ID_t, Entities::PlayerInfo> players;

  bool hasPlayer(ID_t id) const { return players.find(id) != players.end(); }

  std::unordered_map<ID_t, sf::Vector2f> getPlayersPosMap() {
    std::unordered_map<ID_t, sf::Vector2f> poses;

    for (auto &[id, info] : players) {
      poses.emplace(id, info.pos);
    }

    return poses;
  }
};
} // namespace Luntik
