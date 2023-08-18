#pragma once

#include "ClientEntity.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Entities {
class ClientNetworkEntity : ClientEntity {
public:
  ClientNetworkEntity(ID_t id);

  void tick(float deltaTime) override;

  void setPos(const sf::Vector2f &pos) override;
  sf::Vector2f getPos() const override;

private:
  sf::Vector2f m_End;
  sf::Vector2f m_Start;

  float m_T;
};
} // namespace Luntik::Entities
