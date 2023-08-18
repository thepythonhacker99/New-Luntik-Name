#pragma once

#include "../settings.h"
#include "SFML/System/Vector2.hpp"

namespace Luntik::Entities {
class Entity {
public:
  Entity(ID_t id = ID_t_MAX);

  virtual void tick(float deltaTime){};

  ID_t getId() const;

  virtual sf::Vector2f getPos() const { return m_Pos; };
  virtual void setPos(const sf::Vector2f &pos) { m_Pos = pos; };

protected:
  ID_t m_Id;

  sf::Vector2f m_Pos;
};
} // namespace Luntik::Entities
