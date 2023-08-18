#include "ClientNetworkEntity.h"

#include "ClientEntity.h"
#include "SFML/System/Vector2.hpp"
#include <cmath>

namespace Luntik::Entities {
ClientNetworkEntity::ClientNetworkEntity(ID_t id) : ClientEntity(id) {}

void ClientNetworkEntity::tick(float deltaTime) {
  constexpr float tStep = 1.f / Settings::SERVER_TPS;

  m_T += tStep;

  m_Pos.x = std::lerp(m_Start.x, m_End.x, m_T);
  m_Pos.y = std::lerp(m_Start.y, m_End.y, m_T);
}

void ClientNetworkEntity::setPos(const sf::Vector2f &pos) {
  m_End = pos;
  m_T = 0;
}
} // namespace Luntik::Entities
